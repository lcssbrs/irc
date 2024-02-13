#include "../../includes/client/client.hpp"

Client::Client(std::string nickname, std::string username, int fd) : _nickname(nickname), _username(username), _fd(fd)
{
	isCreated = false;
	pass = false;
	nick = false;
	user = false;
	nbmsg = 0;
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

const int		&Client::getNbmsg(void) const
{
	return (nbmsg);
}

const bool		&Client::getCreated(void) const
{
	return (isCreated);
}

const bool		&Client::getPass(void) const
{
	return (pass);
}

const bool		&Client::getNick(void) const
{
	return (nick);
}

const bool		&Client::getUser(void) const
{
	return (user);
}

void				Client::setCreatedtoTrue(void)
{
	isCreated = true;
}
void				Client::setPasstoTrue(void)
{
	pass = true;
}
void				Client::setNicktoTrue(void)
{
	nick = true;
}
void				Client::setUsertoTrue(void)
{
	user = true;
}

void				Client::setNbmsgplusone(void)
{
	nbmsg += 1;
}

void				Client::setNickname(const std::string &nick)
{
	_nickname = nick;
}

void				Client::setUsername(const std::string &user)
{
	_username = user;
}
