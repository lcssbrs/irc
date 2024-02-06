#pragma once

#include <iostream>

class Client {
	private:
		std::string	_nickname;
		std::string	_username;
		int		_fd;

	public:

		Client(std::string &nick, std::string &user, int fd);
		~Client(void);

		//getters
		const std::string	&getNickname(void) const;
		const std::string	&getUsername(void) const;
		const int			&getFd(void) const;
};
