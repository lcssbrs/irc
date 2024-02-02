#include "../../includes/channel/channel.hpp"

Channel::Channel(std::string &name, Client *creator) : _name(name)
{
	_operators[creator->getUsername()] = creator;
	_inviteOnly = false;
	_restrictTopic = false;
	_passwordUse = false;
	_limitUser = false;
}

Channel::~Channel(void) {}

Client	*Channel::kick(Client *user, std::string &name)
{
	if (_operators.find(user->getNickname()) == _operators.end())
		return (NULL); //error : user is not operator
	else if (_operators.find(name) != _operators.end())
		return (NULL); //error : cannot kick operator
	else if (_regulars.find(name) == _regulars.end())
		return (NULL); //error: not a channel member
	else if (_regulars[name] == user)
		return (NULL); //error: user can't kick himself
	return (_regulars[name]);
}

Client	*Channel::invite(Client *user, std::string &name, std::map<int, Client *> &clients)
{
	if (_operators.find(user->getNickname()) == _operators.end())
		return (NULL); //error : user is not operator
	else if (_regulars.find(name) != _regulars.end())
		return (NULL); //error : invited user is already on the channel
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second->getNickname() == name)
			return (it->second);
	}
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
			return ; //error: topic restricted and user not operator tried to change it
	}
	_topic = topic;
}

void	Channel::mode(Client *user, std::string &option, std::string &arg)
{
	if (_operators.find(user->getNickname()) == _operators.end())
		return ; //error: user is not operator
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
		// else error: client not in the channel
	}
	else if (option == "l")
	{
		_limitUser = !_limitUser;
		_nUser = atoi(arg.c_str());
	}
}

void	Channel::userJoin(Client *user)
{
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
