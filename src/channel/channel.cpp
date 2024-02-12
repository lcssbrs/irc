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
	_topic = "No topic set";
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
		std::cout << "ERROR: user is not operator" << std::endl;
		return (NULL); //error : user is not operator
	}
	else if (_operators.find(name) != _operators.end())
	{
		std::cout << "ERROR: cannot kick operator" << std::endl;
		return (NULL); //error : cannot kick operator
	}
	else if (_regulars.find(name) == _regulars.end())
	{
		std::cout << "ERROR: user doesn't exist/isn't in the channel" << std::endl;
		return (NULL); //error: not a channel member
	}
	else if (_regulars[name] == user)
	{
		std::cout << "Error: user can't kick himself" << std::endl;
		return (NULL); //error: user can't kick himself
	}
	Client *temp = _regulars[name];
	_regulars.erase(name);
	return (temp);
}

Client	*Channel::invite(Client *user, std::string &name, std::map<int, Client *> &clients)
{
	if (_operators.find(user->getNickname()) == _operators.end())
	{
		std::cout << "ERROR: user is not operator" << std::endl;
		return (NULL); //error : user is not operator
	}
	else if (_regulars.find(name) != _regulars.end())
	{
		std::cout << "ERROR: User is already in the channel" << std::endl;
		return (NULL); //error : invited user is already on the channel
	}
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second->getNickname() == name)
			return (it->second);
	}
	std::cout << "ERROR: User is not on the server" << std::endl;
	return (NULL); //error: invited user is not on the server
}

void	Channel::topic(void) const
{
	std::cout << _topic << std::endl;
}

void	Channel::topic(Client *user, std::string &topic)
{
	if (_restrictTopic == true)
	{
		if (_operators.find(user->getNickname()) == _operators.end())
		{
			std::cout << "ERROR: only operator can change topic" << std::endl;
			return ;
		}
		//error: topic restricted and user not operator tried to change it
	}
	_topic = topic;
}

void	Channel::mode(Client *user, std::string &option, std::string &arg)
{
	if (_operators.find(user->getNickname()) == _operators.end())
	{
		std::cout << "ERROR: user is not operator" << std::endl;
		return ; //error: user is not operator
	}
	if (option == "i")
		_inviteOnly = !_inviteOnly;
	else if (option == "t")
		_restrictTopic = !_restrictTopic;
	else if (option == "k")
	{
		_password = arg;
		_passwordUse = !_passwordUse;
	}
	else if (option == "o")
	{
		if (_operators.find(arg) != _operators.end())
			_operators.erase(arg);
		else if (_regulars.find(arg) != _regulars.end())
			_operators[arg] = _regulars.find(arg)->second;
		else
			std::cout << "ERROR: client is not in the channel" << std::endl;
	}
	else if (option == "l")
	{
		_limitUser = !_limitUser;
		_nUser = atoi(arg.c_str());
	}
}

void	Channel::userJoin(Client *user, std::string password)
{
	if (_limitUser == true and static_cast<int>(_regulars.size()) == _nUser)
	{
		std::cout << "Channel user limit reached" << std::endl;
		return ;
	}
	else if (_passwordUse == true and password != _password)
	{
		std::cout << "Wrong password" << std::endl;
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
