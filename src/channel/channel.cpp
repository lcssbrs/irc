#include "../../includes/channel/channel.hpp"

void	Channel::printClients(void)
{
	std::cout << "Membres du channel:" << std::endl;
	for (std::map<std::string, Client *>::iterator it = _regulars.begin(); it != _regulars.end(); it++)
			std::cout << it->second->getNickname() << std::endl;
}

Channel::Channel(std::string &name, Client *creator) : _name(name)
{
	_operators[creator->getNickname()] = creator;
	_regulars[creator->getNickname()] = creator;
	_topic = "";
	_inviteOnly = false;
	_restrictTopic = false;
	_passwordUse = false;
	_limitUser = false;
}

Channel::~Channel(void) {}

Client	*Channel::kick(Client *user, std::string &name)
{
	if (_operators.find(user->getNickname()) == _operators.end())
	{
		// 482 ERR_CHANOPRIVSNEEDED
		return (NULL);
	}
	else if (name == "")
	{
		// 461 ERR_NEEDMOREPARAMS
		return (NULL);
	}
	else if (_operators.find(name) != _operators.end())
	{
		std::cout << "ERROR: cannot kick operator" << std::endl; //pas de code pour
		return (NULL);
	}
	else if (_regulars.find(name) == _regulars.end())
	{
		// 442 ERR_NOTONCHANNEL
		return (NULL);
	}
	else if (_regulars[name] == user)
	{
		std::cout << "Error: user can't kick himself" << std::endl; // pas de code pour
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
		// 482 ERR_CHANOPRIVSNEEDED
		return (NULL);
	}
	else if (name == "")
	{
		// 461 ERR_NEEDMOREPARAMS
		return (NULL);
	}
	else if (_regulars.find(name) != _regulars.end())
	{
		//443 ERR_USERONCHANNEL
		return (NULL);
	}
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second->getNickname() == name)
		{
			// 341 RPL_INVITING
			return (it->second);
		}
	}
	// 401 ERR_NOSUCHNICK
	return (NULL);
}

void	Channel::topic(void) const
{
	if (_topic == "")
	{
		// 331 RPL_NOTOPIC
		return ;
	}
	std::cout << _topic << std::endl;
}

void	Channel::topic(Client *user, std::string &topic)
{
	if (_restrictTopic == true)
	{
		if (_operators.find(user->getNickname()) == _operators.end())
		{
			// 482 ERR_CHANOPRIVSNEEDED
			return ;
		}
	}
	_topic = topic;
}

void	Channel::mode(Client *user, std::string &option, std::string &arg)
{
	if (_operators.find(user->getNickname()) == _operators.end())
	{
		// 482 ERR_CHANOPRIVSNEEDED
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
			return ; // 461 ERR_NEEDMOREPARAMS
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
			// 442 ERR_NOTONCHANNEL
			return ;
		}
	}
	else if (option == "l")
	{
		_limitUser = !_limitUser;
		if (_limitUser == true and arg == "")
			return ; // 461 ERR_NEEDMOREPARAMS
		_nUser = atoi(arg.c_str());
	}
	else
		return ;
		// 472 ERR_UNKNOWNMODE
}

void	Channel::userJoin(Client *user, std::string password)
{
	if (_limitUser == true and static_cast<int>(_regulars.size()) == _nUser)
	{
		// 471 ERR_CHANNELISFULL
		return ;
	}
	else if (_passwordUse == true and password != _password)
	{
		// 475 ERR_BADCHANNELKEY
		return ;
	}
	else if (_inviteOnly == true)
	{
		// 473 ERR_INVITEONLYCHAN
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
