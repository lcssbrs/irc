#include "../../includes/client/client.hpp"

Client::Client(std::string nick, std::string user) : _nickname(nick), _username(user) {}

Client::~Client(void) {}

const std::string	&Client::getNickname(void) const
{
	return (_nickname);
}

const std::string	&Client::getUsername(void) const
{
	return (_username);
}
