#ifndef IRC_HPP
#define IRC_HPP

#include "server/server.hpp"
#include "client/client.hpp"
#include "channel/channel.hpp"

void	sendResponse(int fd, std::string code, const std::string &name, std::string error);

#endif
