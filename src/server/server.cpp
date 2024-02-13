#include "../../includes/server/server.hpp"

Server::Server(int port, std::string pass)
{
	this->port = port;
	this->password = pass;
	this->fds.clear();
}

Server::~Server()
{

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
		{
            std::cerr << "Error in poll" << std::endl;
            break;
        }
        if (num_events > 0)
		{
            for (size_t i = 0; i < fds.size(); ++i)
			{
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
							if (buffer.compare(0, 6, "CAP LS") != 0)
								parsing_msg(buffer, fds[i].fd, fds, i);
                        }
						else if (bytes_received == 0)
						{
                            // Déconnexion du client
                            std::cout << "Client disconnected" << std::endl;
                            close(fds[i].fd);
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

void	Server::closeClient(Client & client, std::vector<struct pollfd> fds, int i)
{
	int fd = client.getFd();
	std::cerr << "Client disconnected " << client.getFd() << std::endl;
	close(client.getFd());
	delete (&client);
	clients.erase(clients.find(fd));
	fds.erase(fds.begin() + i);
}

void Server::create_client(std::string & buffer, Client & client, std::vector<struct pollfd> fds, int i)
{
	if (client.getNbmsg() < 2 and !buffer.compare(0, 6, "PASS :") and client.getPass() == false)
	{
		if (buffer.substr(6, buffer.size() - 7) != password)
		{
			closeClient(client, fds, i);
			return ;
		}
		client.setPasstoTrue();
	}
	else if (client.getNbmsg() < 2 and !buffer.compare(0, 4, "NICK") and client.getNickname() == "")
	{
		client.setNickname(buffer.substr(5, buffer.size() - 6));
		std::cout << client.getNickname() << std::endl;
	}
	else if (client.getNbmsg() == 2 && !buffer.compare(0, 4, "USER"))
	{
		client.setUsername(buffer.substr(5, buffer.size() - 6));
		client.setCreatedtoTrue();
		std::cout << client.getUsername() << std::endl;
	}
	else
		closeClient(client, fds, i);
}

void Server::create_channel(std::string & name)
{
	(void)name;
	std::cout << "channel " << name << " created\n";
}

void Server::remove_client_from_channel(Client * kick)
{
	(void)kick;
}

void Server::check_command(std::string buffer, Client * client)
{
	(void)client;
	if (buffer.compare(0, 1, "!") == 0)
	{
		std::string name = buffer.substr(1, buffer.size() - 1);
		create_channel(name);
	}
}

void Server::parsing_msg(std::string & buffer, int fd, std::vector<struct pollfd> fds, int i)
{
	(void)buffer;
	std::map<int, Client *>::iterator findclient;

	findclient = clients.find(fd);
	if (findclient != clients.end())
	{
		if (clients.find(fd) != clients.end() && findclient->second->getCreated() == false)
			create_client(buffer, (*findclient->second), fds, i);
		else if (clients.find(fd) != clients.end() && clients.find(fd)->second->getCreated() == true)
		{
			check_command(buffer, findclient->second);
			std::cout << buffer;
		}
		if (clients.find(fd) != clients.end())
			(*findclient->second).setNbmsgplusone();
	}
	else
		std::cout << "Client not found\n";
}

