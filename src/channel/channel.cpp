#include "../../includes/irc.hpp"

Channel::Channel(std::string &name, std::string password, Client *creator) : _name(name), _password (password)
{
	_operators[creator->getNickname()] = creator;
	_regulars[creator->getNickname()] = creator;
	_topic = "";
	_inviteOnly = false;
	_restrictTopic = true;
	_limitUser = false;
	if (password != "")
		_passwordUse = true;
	else
		_passwordUse = false;
	_nUser = 0;
	std::string msg = ":" + creator->getNickname() + "!" + creator->getUsername() + "@127.0.0.1 JOIN #" + name + "\n";
	send(creator->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);

	msg = ":127.0.0.1 MODE #" + name + " +nt\n";
	send(creator->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);

	msg = msg = ":127.0.0.1 353 " + creator->getNickname() + " = #" + _name + " :@" + creator->getNickname() + "\n";
	send(creator->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);

	msg = ":127.0.0.1 366 " + creator->getNickname() + " #" + name + " :End of /NAMES list.\n";
	send(creator->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
}

Channel::~Channel(void) {}

void Channel::kick(Client *user, std::string &name)
{
	if (_regulars.find(user->getNickname()) == _regulars.end())
		sendResponse(user->getFd(), "442", user->getNickname(), "");
	else if (_operators.find(user->getNickname()) == _operators.end())
		sendResponse(user->getFd(), "482", user->getNickname(), "");
	else if (name == "")
		sendResponse(user->getFd(), "461", user->getNickname(), "");
	else if (_operators.find(name) != _operators.end())
	{
		std::string msg = ":127.0.0.1 Error :You can't kick an operator\n";
		send(user->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
	}
	else if (_regulars.find(name) == _regulars.end())
		sendResponse(user->getFd(), "442", user->getNickname(), "");
	else if (_regulars[name] == user)
	{
		std::string msg = ":127.0.0.1 Error :You can't kick yourself\n";
		send(user->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
	}
	else
	{
		std::string	msg = ":"  + user->getNickname() + "!" + user->getNickname() + "@127.0.0.1 KICK #" + _name + _regulars[name]->getNickname() + ":\n";
		sendAll(msg);
		_regulars.erase(name);
	}
}

Client	*Channel::invite(Client *user, std::string &name, std::map<int, Client *> &clients)
{
	if (_regulars.find(user->getNickname()) == _regulars.end())
	{
		sendResponse(user->getFd(), "442", user->getNickname(), "");
		return (NULL);
	}
	else if (_operators.find(user->getNickname()) == _operators.end())
	{
		sendResponse(user->getFd(), "482", user->getNickname(), "");
		return (NULL);
	}
	else if (name == "")
	{
		sendResponse(user->getFd(), "461", user->getNickname(), "");
		return (NULL);
	}
	else if (_regulars.find(name) != _regulars.end())
	{
		sendResponse(user->getFd(), "443", user->getNickname(), "");
		return (NULL);
	}
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second->getNickname() == name)
		{
			sendResponse(user->getFd(), "341", user->getNickname(), "");
			return (it->second);
		}
	}
	sendResponse(user->getFd(), "401", user->getNickname(), "");
	return (NULL);
}

void	Channel::topic(Client *user) const
{
	if (_regulars.find(user->getNickname()) == _regulars.end())
	{
		sendResponse(user->getFd(), "442", user->getNickname(), "");
		return ;
	}
	if (_topic == "")
	{
		sendResponse(user->getFd(), "331", user->getNickname(), "");
		return ;
	}
	send(user->getFd(), _topic.c_str(), _topic.size(), MSG_CONFIRM);
}

void	Channel::topic(Client *user, std::string &topic)
{
	if (_regulars.find(user->getNickname()) == _regulars.end())
	{
		sendResponse(user->getFd(), "442", user->getNickname(), "");
		return ;
	}
	if (_restrictTopic == true)
	{
		if (_operators.find(user->getNickname()) == _operators.end())
		{
			sendResponse(user->getFd(), "482", user->getNickname(), "");
			return ;
		}
	}
	_topic = topic;
}

void	Channel::mode(Client *user, bool change, std::string &option, std::string &arg)
{
	if (option == "b" or option == "")
		return ;
	if (_regulars.find(user->getNickname()) == _regulars.end())
	{
		sendResponse(user->getFd(), "442", user->getNickname(), "");
		return ;
	}
	if (_operators.find(user->getNickname()) == _operators.end())
	{
		sendResponse(user->getFd(), "482", user->getNickname(), "");
		return ;
	}
	if (option == "i")
	{
		_inviteOnly = change;
		std::string msg = ":" + user->getNickname() + "!" + user->getNickname() + "@127.0.0.1 MODE #" + _name + ' ';
		if (change == true)
			msg += "+i\n";
		else
			msg += "-i\n";
		sendAll(msg);

	}
	else if (option == "t")
	{
		_restrictTopic = change;
		std::string msg = ":" + user->getNickname() + "!" + user->getNickname() + "@127.0.0.1 MODE #" + _name + ' ';
		if (change == true)
			msg += "+t\n";
		else
			msg += "-t\n";
		sendAll(msg);
	}
	else if (option == "k")
	{
		_passwordUse = change;
		if (_passwordUse == true and _password == "" and arg == "")
		{
			sendResponse(user->getFd(), "461", user->getNickname(), "");
			_passwordUse = false;
			return ;
		}
		_password = arg;
		std::string msg = ":" + user->getNickname() + "!" + user->getNickname() + "@127.0.0.1 MODE #" + _name + ' ';
		if (change == true)
			msg += "+k " + _password + '\n';
		else
			msg += "-k\n";
		sendAll(msg);
	}
	else if (option == "o")
	{
		if (change == true)
		{
			if (_operators.find(arg) != _operators.end())
			{
				std::string msg = ":127.0.0.1 Error :User is already operator\n";
				send(user->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
			}
			else if (_regulars.find(arg) != _regulars.end())
			{
				_operators[arg] = _regulars[arg];
				std::string msg = ":" + user->getNickname() + "!" + user->getNickname() + "@127.0.0.1 MODE #" + _name + " +o " + _regulars[arg]->getNickname() + "\n";
				sendAll(msg);
			}
			else
			{
				sendResponse(user->getFd(), "443", user->getNickname(), "");
				return ;
			}
		}
		else
		{
			if (_operators.find(arg) != _operators.end())
			{
				_operators.erase(arg);
				std::string msg = ":" + user->getNickname() + "!" + user->getNickname() + "@127.0.0.1 MODE #" + _name + " -o " + _regulars[arg]->getNickname() + "\n";
				sendAll(msg);
			}
			else if (_regulars.find(arg) != _regulars.end())
			{
				std::string msg = ":127.0.0.1 Error :User is not an operator\n";
				send(user->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
			}
			else
			{
				std::string msg = ":127.0.0.1 Error :User is not a channel member\n";
				send(user->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
			}
		}
	}
	else if (option == "l")
	{
		_limitUser = change;
		if (_limitUser == true)
		{
			if (arg == "")
			{
				sendResponse(user->getFd(), "461", user->getNickname(), "");
				_limitUser = false;
				return ;
			}
			else if (arg.find_first_not_of("+-0123456789") != std::string::npos)
			{
				std::string msg = ":127.0.0.1 Error :Invalid argument (only takes numbers, + and -)\n";
				send(user->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
				_limitUser = false;
				return ;
			}
			int i = atoi(arg.c_str());
			if (i <= 0)
			{
				std::string msg = ":127.0.0.1 Error :Invalid user limit (it needs to be more than 0)\n";
				send(user->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
				_limitUser = false;
				return ;
			}
			else if (i < static_cast<int>(_regulars.size()))
			{
				std::string msg = ":127.0.0.1 Error :Invalid user limit (it needs to be more than the number of the channel's clients)";
				send(user->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
				_limitUser = false;
				return ;
			}
			_nUser = i;
			std::string msg = ":" + user->getNickname() + "!" + user->getNickname() + "@127.0.0.1 MODE #" + _name + ' ';
			if (change == true)
				msg += "+l " + arg + '\n';
			else
				msg += "-l\n";
			sendAll(msg);
		}
	}
	else
		sendResponse(user->getFd(), "472", user->getNickname(), "");
}

void	Channel::userJoin(Client *user, std::string password)
{
	if (_limitUser == true and static_cast<int>(_regulars.size()) == _nUser)
	{
		sendResponse(user->getFd(), "471", user->getNickname(), "");
		return ;
	}
	else if (_passwordUse == true and password != _password)
	{
		sendResponse(user->getFd(), "475", user->getNickname(), "");
		return ;
	}
	else if (_inviteOnly == true)
	{
		std::string msg = ":127.0.0.1 " + user->getNickname() + " #" + _name + " :Cannot join channel, you must be invited\n";
		send(user->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
		return ;
	}
	if (user)
	{
		sendNewcomer(user);
		std::string msg = ":" + user->getNickname() + "!" + user->getUsername() + "@127.0.0.1 JOIN #" + _name + "\n";
		send(user->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);

		if (_topic != "")
			msg = ":127.0.0.1 332 " + user->getNickname() + " #" + _name + " :" + _topic + "\n";
		else
			msg = ":127.0.0.1 331 " + user->getNickname() + " #" + _name + " :No topic set\n";
		send(user->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);

		msg = ":127.0.0.1 333 " + user->getNickname() + " #" + _name + " " + _operators.begin()->second->getNickname() + "!~" + _operators.begin()->second->getNickname()[0] + "@127.0.0.1 1547691506\n";
		send(user->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);

		msg = ":127.0.0.1 353 " + user->getNickname() + " = #" + _name + " :";
		for (std::map<std::string, Client *>::iterator it = _regulars.begin(); it != _regulars.end(); it++)
		{
			if (_operators.find(it->second->getNickname()) != _operators.end())
				msg += "@";
			msg += it->second->getNickname() + " ";
		}
		msg += user->getNickname() + "\n";
		send(user->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);

		msg = ":127.0.0.1 366 " + user->getNickname() + " #" + _name + " :End of /NAMES list.\n";
		send(user->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
		_regulars[user->getNickname()] = user;
	}
}

void	Channel::sendNewcomer(Client *user)
{
	std::string msg = ":" + user->getNickname() + "!" + user->getUsername() + "@127.0.0.1 JOIN :#" + _name + "\n";
	for (std::map<std::string, Client *>::iterator it = _regulars.begin(); it != _regulars.end(); it++)
		send(it->second->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
}

int		Channel::userLeave(Client *user, std::string msg)
{
	if (_operators.find(user->getNickname()) != _operators.end())
	{
		_operators.erase(user->getNickname());
		_regulars.erase(user->getNickname());
		if (_operators.size() == 0)
		{
			if (_regulars.size() == 0)
				return (1); //last user left need to close the channel
			else
				_operators[_regulars.begin()->first] = _regulars.begin()->second; //latest user who joined the channel become the operator become last operator left
		}
		return (0);
	}
	_regulars.erase(user->getNickname());
	sendLeave(user, msg);
	return (0);
}

void	Channel::sendLeave(Client *user, std::string &msg)
{
	std::string res = ":" + user->getNickname() + "!" + user->getUsername() + "@127.0.0.1 PART #" + _name + msg;
	for (std::map<std::string, Client *>::iterator it = _regulars.begin(); it != _regulars.end(); it++)
		send(it->second->getFd(), res.c_str(), res.size(), MSG_CONFIRM);
}

void	Channel::sendMessage(Client *user, std::string msg)
{
	if (_regulars.find(user->getNickname()) == _regulars.end())
	{
		sendResponse(user->getFd(), "442", user->getNickname(), "");
		return ;
	}
	std::string res = ":" + user->getNickname() + "!" + user->getUsername() + "@127.0.0.1 PRIVMSG #" + _name + " :" + msg + "\n";
	for (std::map<std::string, Client *>::iterator it = _regulars.begin(); it != _regulars.end(); it++)
	{
		if (it->second != user)
			send(it->second->getFd(), res.c_str(), res.size(), MSG_CONFIRM);
	}
}

void	Channel::sendAll(std::string &msg)
{
	for (std::map<std::string, Client *>::iterator it = _regulars.begin(); it != _regulars.end(); it++)
		send(it->second->getFd(), msg.c_str(), msg.size(), MSG_CONFIRM);
}

const std::string	&Channel::getName(void) const
{
	return (_name);
}


std::map<std::string, Client *> & Channel::getOpe(void)
{
	return _operators;
}

std::map<std::string, Client *> & Channel::getReg(void)
{
	return _regulars;
}