#include "../../includes/irc.hpp"

void	Channel::printClients(void)
{
	std::cout << "Membres du channel:" << std::endl;
	for (std::map<std::string, Client *>::iterator it = _regulars.begin(); it != _regulars.end(); it++)
			std::cout << it->second->getNickname() << std::endl;
}

Channel::Channel(std::string &name, std::string password,  Client *creator) : _name(name), _password (password)
{
	_operators[creator->getNickname()] = creator;
	_regulars[creator->getNickname()] = creator;
	_topic = "";
	_inviteOnly = false;
	_restrictTopic = false;
	_limitUser = false;
	if (password != "")
		_passwordUse = true;
	else
		_passwordUse = false;
	_nUser = 0;
}

Channel::~Channel(void) {}

Client	*Channel::kick(Client *user, std::string &name)
{
	if (_operators.find(user->getNickname()) == _operators.end())
	{
		sendResponse(user->getFd(), "482", user->getNickname(), "");
		return (NULL);
	}
	else if (name == "")
	{
		sendResponse(user->getFd(), "461", user->getNickname(), "");
		return (NULL);
	}
	else if (_operators.find(name) != _operators.end())
	{
		// write(user->getFd(), "ERROR: You can't kick an operator", 30); /!\ ERROR pas déjà présente sur IRC, voir quoi faire pour le code
		return (NULL);
	}
	else if (_regulars.find(name) == _regulars.end())
	{
		sendResponse(user->getFd(), "442", user->getNickname(), "");
		return (NULL);
	}
	else if (_regulars[name] == user)
	{
		// write(user->getFd(), "ERROR: You can't kick yourself", 30); /!\ ERROR pas déjà présente sur IRC, voir quoi faire pour le code
		return (NULL);
	}
	Client *temp = _regulars[name];
	_regulars.erase(name);
	return (temp);
}

Client	*Channel::invite(Client *user, std::string &name, std::map<int, Client *> &clients)
{
	if (_operators.find(user->getNickname()) == _operators.end())
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
	if (_topic == "")
	{
		sendResponse(user->getFd(), "331", user->getNickname(), "");
		return ;
	}
	send(user->getFd(), _topic.c_str(), _topic.size(), MSG_CONFIRM);
}

void	Channel::topic(Client *user, std::string &topic)
{
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

void	Channel::mode(Client *user, std::string &option, std::string &arg)
{
	if (_operators.find(user->getNickname()) == _operators.end())
	{
		sendResponse(user->getFd(), "482", user->getNickname(), "");
		return ;
	}
	if (option == "i")
		_inviteOnly = !_inviteOnly;
	else if (option == "t")
		_restrictTopic = !_restrictTopic;
	else if (option == "k")
	{
		_passwordUse = !_passwordUse;
		if (_passwordUse == true and arg == "")
		{
			sendResponse(user->getFd(), "461", user->getNickname(), "");
			_passwordUse = !_passwordUse;
			return ;
		}
		_password = arg;
	}
	else if (option == "o")
	{
		if (_operators.find(arg) != _operators.end())
			_operators.erase(arg);
		else if (_regulars.find(arg) != _regulars.end())
			_operators[arg] = _regulars.find(arg)->second;
		else
		{
			sendResponse(user->getFd(), "442", user->getNickname(), "");
			return ;
		}
	}
	else if (option == "l")
	{
		_limitUser = !_limitUser;
		if (_limitUser == true)
		{
			if (arg == "")
			{
				sendResponse(user->getFd(), "461", user->getNickname(), "");
				_limitUser = !_limitUser;
				return ;
			}
			int i = atoi(arg.c_str());
			if (i <= 0)
			{
				// write(user->getFd(), "ERROR: Invalid user limit (it needs to be more than 0)", 54); /!\ ERROR pas déjà présente sur IRC, voir quoi faire pour le code
				_limitUser = !_limitUser;
				return ;
			}
			else if (i < static_cast<int>(_regulars.size()))
			{
				// write(user->getFd(), "ERROR: Invalid user limit (it needs to be more than the number of the channel's clients)", 88); /!\ ERROR pas déjà présente sur IRC, voir quoi faire pour le code
				_limitUser = !_limitUser;
				return ;
			}
			_nUser = i;
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
		sendResponse(user->getFd(), "473", user->getNickname(), "");
		return ;
	}
	if (user)
		_regulars[user->getNickname()] = user;
}

int		Channel::userLeave(Client *user)
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
	return (0);
}

void	Channel::printStatus(void)
{
	std::cout << "Channel: " << _name << std::endl;
	std::cout << "Invite only: " << _inviteOnly << std::endl;
	std::cout << "Restricted topic: " << _restrictTopic << " (" << _topic << ")" << std::endl;
	std::cout << "Password needed: " << _passwordUse << " (" << _password << ")" << std::endl;
	std::cout << "Limit user: " << _limitUser << " (" << _nUser << ")" << std::endl;
	std::cout << "Operators: " << std::endl;
	for (std::map<std::string, Client *>::iterator it = _operators.begin(); it != _operators.end(); it++)
		std::cout << it->second->getNickname() << std::endl;
}
