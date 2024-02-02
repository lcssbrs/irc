#pragma once

#include <iostream>

class Client {
	private:
		std::string	_nickname;
		std::string	_username;

	public:

		Client(std::string nick, std::string user);
		~Client(void);

		//getters
		const std::string	&getNickname(void) const;
		const std::string	&getUsername(void) const;
};
