#include "../../includes/server/server.hpp"

void	sendResponse(int fd, std::string code, const std::string &name, std::string error)
{
	std::string msg = ":127.0.0.1 " + code + " " + name + " :" + error + "\n";
	send(fd, msg.c_str(), msg.size(), MSG_CONFIRM);
}

Server::Server(int port, std::string pass)
{
	this->port = port;
	this->password = pass;
	this->fds.clear();
}

Server::~Server()
{
	std::string msg = ":127.0.0.1 QUIT :Server has been closed. See you next time!\n";
	std::map<int, Client *>::iterator it = clients.begin();
	while (it != clients.end())
	{
		send(it->first, msg.c_str(), msg.size(), MSG_CONFIRM);
		delete it->second;
		it++;
	}
	clients.clear();
	std::map<std::string, Channel *>::iterator ite = channels.begin();
	while (ite != channels.end())
	{
		delete ite->second;
		ite++;
	}
	channels.clear();
}

const char *Server::BindException::what() const throw()
{
	return "Error bindage";
}

const char *Server::ListenException::what() const throw()
{
	return "Error listnening";
}

const char *Server::TCPException::what() const throw()
{
	return "Error protocol";
}

const char *Server::SocketException::what() const throw()
{
	return "Error socket server";
}

void Server::init_server()
{
	struct sockaddr_in	sin;
	struct protoent *pe = getprotobyname("tcp");
	if (pe == NULL)
		throw (TCPException());
	this->fd_server = socket(PF_INET, SOCK_STREAM, pe->p_proto);
	if (this->fd_server == -1)
		throw (SocketException());
	sin.sin_family = AF_INET; //ipv4
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(this->port);
	int yes=1;
	if (setsockopt(this->fd_server, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
	{
		std::cerr << "error reuse bind\n";
		exit(1);
	}
	if (bind(this->fd_server, (struct sockaddr*)&sin, sizeof(sin)) == -1)
		throw (BindException());
	if (listen(this->fd_server, backlog) == -1)
		throw (ListenException());
	struct pollfd pollfd;
	this->fds.push_back(pollfd);
	this->fds.front().fd = fd_server;
	this->fds.front().events = POLLIN;
}

static int get_line(int fd, std::string &line)
{
	char chr[2] = {0};
	int readed = 0;
	int total_read = 0;;
	while ((readed = recv(fd,chr, 1, 0)) > 0)
	{
		total_read += readed;
		std::string append(chr);
		line += append;
		if (chr[0] == '\n')
			break;
		memset(chr, 0, 2);
	}
	return total_read;
}

void Server::manage_loop()
{
    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
    std::string line[backlog]; // Tableau de lignes pour chaque descripteur de fichier
	line->clear();
    while (true)
	{
        int num_events = poll(&fds.front(), fds.size(), -1);
        if (num_events == -1)
            break;
        if (num_events > 0)
		{
            for (size_t i = 0; i < fds.size(); ++i)
			{
				//if (clients.find(fds[i].fd) != clients.end())
				//	send_ping(clients.find(fds[i].fd)->second);
                if (fds[i].revents & POLLIN)
				{
                    if (fds[i].fd == fd_server)
					{
                        // Nouvelle connexion entrante
                        int client_fd = accept(fd_server, (struct sockaddr*)&sin, &len);
                        if (client_fd == -1)
						{
                            std::cerr << "Error accepting connection" << std::endl;
                        }
						else
						{
                            // Ajouter le nouveau client à la liste des descripteurs de fichiers à surveiller
                            struct pollfd client_pollfd;
                            client_pollfd.fd = client_fd;
                            client_pollfd.events = POLLIN;
							client_pollfd.revents = POLLIN;
                            fds.push_back(client_pollfd);
							clients[client_fd] = new Client ("", "", client_fd);
                        }
                    }
					else
					{
                        // Données disponibles sur un client existant
                        std::string buffer;
                        int bytes_received = get_line(fds[i].fd, buffer);
                        if (bytes_received > 0)
						{
                            // Traitement des données reçues
                            line[fds[i].fd] += buffer;
							parsing_msg(buffer, fds[i].fd, i);
                        }
						else if (bytes_received == 0)
						{
                            // Déconnexion du client
                            std::cerr << "Client " << clients.find(fds[i].fd)->second->getNickname() << "!" << clients.find(fds[i].fd)->second->getUsername() << " has leaved the server." << std::endl;
                            close(fds[i].fd);
							std::string name = clients.find(fds[i].fd)->second->getNickname();
							std::map<std::string, Channel *>::iterator it = channels.begin();
							while (it != channels.end())
							{
								while (it->second->getReg().find(name) != it->second->getReg().end())
								{
									if (it->second->userLeave(clients.find(fds[i].fd)->second, " :Disconnected server\n") == 1)
									{
										delete channels[name];
										channels.erase(name);
									}
								}
								it++;
							}
							delete (clients.find(fds[i].fd)->second);
							clients.erase(fds[i].fd);
                            fds.erase(fds.begin() + i);
                        }
						else
						{
                            // Erreur de réception
                            std::cerr << "Error receiving data from client" << std::endl;
                    	}
					}
                }
            }
        }
    }
}

int Server::manage_server()
{
	try
	{
		init_server();
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		exit(1);
	}
	manage_loop();
	return (0);
}

void	Server::closeClient(Client & client, int i)
{
	int fd = client.getFd();
	std::cerr << "Client " << fd << " has been kicked from the server." << std::endl;
	close(fd);
	delete (&client);
	clients.erase(clients.find(fd));
	fds.erase(fds.begin() + i);
}

void Server::create_client(std::string & buffer, Client & client, int i)
{
	if (!buffer.compare(0, 6, "CAP LS"))
		return ;
	if (!buffer.compare(0, 6, "PASS :") and client.getPass() == false)
	{
		if (buffer.substr(6, buffer.size() - 7) != password)
		{
			sendResponse(client.getFd(), "464", client.getNickname(), "Password Incorrect");
			send(client.getFd(), "ERROR :Closing Link: localhost (Bad Password)\n", 46, MSG_CONFIRM);
			closeClient(client, i);
			return ;
		}
		client.setPasstoTrue();
	}
	else if (!buffer.compare(0, 4, "NICK") and client.getPass() == true)
	{
		if (checkNickname(buffer.substr(5, buffer.size() - 6), client.getFd()))
			return ;
		client.setNickname(buffer.substr(5, buffer.size() - 6));
	}
	else if (!buffer.compare(0, 4, "USER") and client.getPass() == true)
		client.setUsername(buffer.substr(buffer.find(':') + 1, buffer.size() - buffer.find(':') - 2));
	else
	{
		if (client.getPass() == false)
		{
			sendResponse(client.getFd(), "464", client.getNickname(), "Password Incorrect");
			send(client.getFd(), "ERROR :Closing Link: localhost (Bad Password)\n", 46, MSG_CONFIRM);
		}
		closeClient(client, i);
	}
	if (client.getPass() == true and client.getNickname() != "" and client.getUsername() != "")
	{
		client.setCreatedtoTrue();
		std::string msg = ":127.0.0.1 001 " + client.getNickname() + " :" + "Welcome to the Internet Relay Network of 42, " + client.getNickname() + "!" + client.getUsername() + "@127.0.0.1!\n";
		send(client.getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
		sendInfo(&client);
		std::cout << "Client " << client.getNickname() << "!" << client.getUsername() << " connected!" << std::endl;
	}
}

void Server::create_channel(std::string name, Client * client)
{
	if (name.compare(0, 1, "#") != 0 or name.compare(1, 1, " ") == 0)
	{
		sendResponse(client->getFd(), "461", client->getNickname(), "");
		return ;
	}
	name = name.substr(1, name.size() - 1);
	size_t lenName = name.find(" ");
	std::string newName = name.substr(0, lenName);
	std::string password = name.substr(lenName + 1, name.size() - (lenName));
	if (password == newName)
		password = "";
	if (channels.find(newName) == channels.end())
	{
		std::cout << "Channel " << newName << " created by " << client->getNickname() << '\n' ;
		channels[newName] = new Channel(newName, password, client);
	}
	else if (channels.find(newName) != channels.end())
		channels.find(newName)->second->userJoin(client, password);
}

void Server::remove_client_from_channel(Client *user, std::string arg)
{
	std::string name = arg.substr(1, arg.find(' ') - 1);
	if (name.find('\n') != std::string::npos)
		name.resize(name.size() - 1);
	std::string msg = "";
	if (name.size() + 1 < arg.size())
		msg = arg.substr(name.size() + 1);
	if (channels.find(name) != channels.end())
	{
		if (channels[name]->userLeave(user, msg) == 1)
		{
			delete channels[name];
			channels.erase(name);
			std::cout << "Channel " << name << " has been erased." << std::endl;
		}
	}
	else
	{
		std::string msg = ":127.0.0.1 403 " + user->getNickname() + " #" + name + '\n';
		send(user->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
	}
}

void	Server::sendmessagetoclient(Client *client, std::string buffer)
{
	if (buffer.find('#') < buffer.find(':'))
	{
		std::string name = buffer.substr(9, buffer.find(' ', 9) - 9);
		std::map<std::string, Channel *>::iterator it = channels.begin();
		while (it != channels.end())
		{
			if (it->second->getName() == name)
			{
				it->second->sendMessage(client, buffer.substr(buffer.find(':') + 1));
				return ;
			}
			it++;
		}
		std::string msg = ":127.0.0.1 403 " + client->getNickname() + " #" + name + '\n';
		send(client->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
	}
	else
	{
		std::string name = buffer.substr(8, buffer.find(' ', 8) - 8);
		std::string msg = ":" + client->getNickname() + "!~" + client->getNickname()[0] + "@127.0.0.1 " + buffer;
		std::map<int, Client *>::iterator it = clients.begin();
		while (it != clients.end())
		{
			if (it->second->getNickname().compare(name) == 0)
			{
				send(it->second->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
				return ;
			}
			it++;
		}
		msg = ":127.0.0.1 401 " + client->getNickname() + ' ' + name + '\n';
		send(client->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
	}
}

void Server::parsing_msg(std::string & buffer, int fd, int i)
{
	std::map<int, Client *>::iterator findclient;

	findclient = clients.find(fd);
	if (findclient != clients.end())
	{
		if (findclient->second->getCreated() == false)
			create_client(buffer, (*findclient->second), i);
		else
		{
			if (buffer.compare(0, 1, "!") == 0)
			{
				std::string name = buffer.substr(1, buffer.size() - 2);
				create_channel(name, findclient->second);
			}
			else if (buffer.compare(0, 8, "PRIVMSG ") == 0)
				sendmessagetoclient(findclient->second, buffer);
			else if (buffer.compare(0, 5, "JOIN ") == 0)
				create_channel(buffer.substr(5, buffer.size() - 6), findclient->second);
			else if (buffer.compare(0, 5, "MODE ") == 0)
				mode_channel(buffer.substr(5, buffer.size() - 6), findclient->second);
			else if (buffer.compare(0, 4, "PING") == 0)
			{
				std::string ping_param = buffer.substr(5); // Récupérez le paramètre du message PING
				std::string pong_message = "PONG " + ping_param + "\n"; // Construisez le message PONG
				send(findclient->second->getFd(), pong_message.c_str(), pong_message.size(), MSG_CONFIRM); // Envoyez le message PONG au client
			}

			else if (buffer.compare(0, 5, "KICK ") == 0)
				ft_kick(findclient->second, buffer.substr(5, buffer.size() - 6));
			else if (buffer.compare(0, 7, "INVITE ") == 0)
				ft_invite(findclient->second, buffer.substr(7, buffer.size() - 8));
			else if (buffer.compare(0, 6, "TOPIC ") == 0)
				ft_topic(findclient->second, buffer.substr(6, buffer.size() - 7));
			else if (buffer.compare(0, 5, "PART ") == 0)
				remove_client_from_channel(findclient->second, buffer.substr(5));
		}
	}
	else
		std::cout << "Client not found\n";
}

void	Server::mode_channel(std::string channel, Client * client)
{
	if (channel.compare(0, 1, "#") != 0 or channel.compare(1, 1, " ") == 0)
	{
		sendResponse(client->getFd(), "461", client->getNickname(), "");
		return ;
	}
	channel = channel.substr(1, channel.size() - 1);
	size_t lenName = channel.find(" ");
	std::string name = channel.substr(0, lenName);
	std::string mode = channel.substr(lenName + 1, channel.size() - (lenName));
	bool boole = true;
	size_t lenMode = mode.find(" ");
	mode = mode.substr(0, mode.size() - lenMode);
	if (mode == name)
		mode = "";
	std::string arg = channel;
	for(int i = 0; i < 2 ;i++)
		arg = arg.substr(arg.find(" ") + 1, arg.size() - arg.find(" "));
	if (channel.find(" ") == channel.rfind(" "))
		arg = " ";
	if (arg == mode)
		arg = "";
	if (mode.compare(0, 1, "-") == 0)
		boole = false;
	if (mode.compare(0, 1, "+") == 0 or mode.compare(0, 1, "-") == 0)
		mode = mode.substr(1, lenMode - 1);
	if (channels.find(name) != channels.end())
		channels.find(name)->second->mode(client, boole, mode, arg);
	else
	{
		std::string msg = ":127.0.0.1 403 " + client->getNickname() + " #" + name + '\n';
		send(client->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
	}
}

int	Server::checkNickname(std::string nick, int fd)
{
	if (nick.find(" ") != std::string::npos || nick.find(",") != std::string::npos || nick.find("*") != std::string::npos || nick.find("!") != std::string::npos || nick.find("?") != std::string::npos || nick.find("@") != std::string::npos || nick.find(".") != std::string::npos)
	{
		sendResponse(fd, "433", nick, "Erroneous Nickname");
		return (1);
	}
	else if (nick.compare(0, 1, "$") == 0 || nick.compare(0, 1, ":") == 0 || nick.compare(0, 1, "#") == 0 || nick.compare(0, 1, "&") == 0 || nick.compare(0, 1, "$") == 0 || nick.compare(0, 1, "~") == 0 || nick.compare(0, 1, "+") == 0)
	{
		sendResponse(fd, "433", nick, "Erroneous Nickname");
		return (1);
	}
	else if (nick == "")
	{
		sendResponse(fd, "433", nick, "Erroneous Nickname");
		return (1);
	}
	for (std::vector<struct pollfd>::iterator it = fds.begin() + 1; it != fds.end(); it++)
	{
		if (fd != it->fd and clients[it->fd]->getNickname() == nick)
		{
			sendResponse(fd, "433", nick, "Erroneous Nickname");
			return (1);
		}
	}
	return (0);
}

void	Server::ft_kick(Client * client, std::string buffer)
{
	if (buffer.compare(0, 1, "#") != 0 or buffer.compare(1, 1, " ") == 0)
	{
		sendResponse(client->getFd(), "461", client->getNickname(), "");
		return ;
	}
	buffer = buffer.substr(1, buffer.size() - 1);
	std::string channel = buffer.substr(0, buffer.find(" "));
	buffer = buffer.substr(buffer.find(" ") + 1, buffer.size() - (buffer.find(" ") + 1));
	std::string nickname = buffer.substr(0, buffer.find(" "));
	if (nickname == channel)
		nickname = "";
	if (channels.find(channel) != channels.end())
		channels.find(channel)->second->kick(client, nickname);
	else
	{
		std::string msg = ":127.0.0.1 403 " + client->getNickname() + " #" + channel + '\n';
		send(client->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
	}
}

void	Server::send_ping(Client * client)
{
	time_t newTime = time(NULL);
	if (difftime(newTime, client->getTime()) >= 5)
	{
		write (client->getFd(), "PING 10\n", 8);
		client->setTime(newTime);
	}
	return ;
}

void Server::ft_invite(Client *client, std::string buffer)
{
	std::string name;
	if (buffer.find("#") == std::string::npos or buffer.compare(1, 1, " ") == 0)
	{
		sendResponse(client->getFd(), "461", client->getNickname(), "");
		return ;
	}
	else
		name = buffer.substr(buffer.find("#") + 1, buffer.find(" ") - (buffer.find("#") + 1));
	std::string iencli = buffer.substr(name.size() + 2, buffer.find(" "));
	std::cout << iencli << std::endl;
	if(channels.find(name) != channels.end())
		channels.find(name)->second->invite(client, iencli, clients);
	else
	{
		std::string msg = ":127.0.0.1 403 " + client->getNickname() + " #" + name + '\n';
		send(client->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
	}
}

void	Server::ft_topic(Client * client, std::string buffer)
{
	if (buffer.compare(0, 1, "#") != 0 or buffer.compare(1, 1, " ") == 0)
	{
		sendResponse(client->getFd(), "461", client->getNickname(), "");
		return ;
	}
	buffer = buffer.substr(1, buffer.size() - 1);
	std::string name = buffer.substr(0, buffer.find(" "));
	std::string topic = "";
	if (buffer.find(":") != std::string::npos)
		topic = buffer.substr(buffer.find(":") + 1, buffer.size() - (buffer.find(":") + 1));
	if (channels.find(name) != channels.end())
	{
		if (topic == "")
			channels.find(name)->second->topic(client);
		else
			channels.find(name)->second->topic(client, topic);
	}
	else
	{
		std::string msg = ":127.0.0.1 403 " + client->getNickname() + " #" + name + '\n';
		send(client->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
	}
}

void	Server::sendInfo(Client *user)
{
	std::string	msg = ":IRCserver@127.0.0.1 PRIVMSG :Hi " + user->getNickname() + "! Here are the useful commands present on our server:\n";
	send(user->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
	msg = ":IRCserver@127.0.0.1 PRIVMSG :- JOIN: allows you to join a channel or create it if it does not exist.\n";
	send(user->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
	msg = ":IRCserver@127.0.0.1 PRIVMSG :- PART: allows you to leave a channel.\n";
	send(user->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
	msg = ":IRCserver@127.0.0.1 PRIVMSG :- PRIVMSG: allows you to send a message to a user or a channel.\n";
	send(user->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
	msg = ":IRCserver@127.0.0.1 PRIVMSG :- KICK: allows you to kick a user of a channel.\n";
	send(user->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
	msg = ":IRCserver@127.0.0.1 PRIVMSG :- INVITE: allows you to invite a user to a channel.\n";
	send(user->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
	msg = ":IRCserver@127.0.0.1 PRIVMSG :- MODE: allows you to change the parameters of a channel: enable/disable password (k), enable/disable user limit (l), enable/disable invite-only (i), restrict/ unrestrict the topic (t) or promote/demote the operator title.\n";
	send(user->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
	msg = ":IRCserver@127.0.0.1 PRIVMSG :- TOPIC: allows you to see/define the topic of a channel.\n";
	send(user->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
	msg = ":IRCserver@127.0.0.1 PRIVMSG :Have fun and enjoy your time here!\n";
	send(user->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
}
