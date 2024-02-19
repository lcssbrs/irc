#include "../../includes/client/client.hpp"

Client::Client(std::string nickname, std::string username, int fd) : _nickname(nickname), _username(username), _fd(fd)
{
	isCreated = false;
	pass = false;
	_time = time(NULL);
	_time_ = time(NULL);
}

Client::~Client(void) {}

const std::string	&Client::getNickname(void) const
{
	return (_nickname);
}

const std::string	&Client::getUsername(void) const
{
	return (_username);
}

const int		&Client::getFd(void) const
{
	return (_fd);
}

const bool		&Client::getCreated(void) const
{
	return (isCreated);
}

const bool		&Client::getPass(void) const
{
	return (pass);
}

void				Client::setCreatedtoTrue(void)
{
	isCreated = true;
}

void				Client::setPasstoTrue(void)
{
	pass = true;
}

void				Client::setNickname(const std::string &nick)
{
	_nickname = nick;
}

void				Client::setUsername(const std::string &user)
{
	_username = user;
}

time_t				Client::getTime(void)const
{
	return _time;
}

void				Client::setTime(time_t newTime)
{
	_time = newTime;
}

time_t				Client::getTimeping(void)const
{
	return _time_;
}

void				Client::setTimeping(time_t newTime)
{
	_time_ = newTime;
}
