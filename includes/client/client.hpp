#pragma once

#include <iostream>

class Client {
	private:
		std::string	_nickname;
		std::string	_username;
		int		_fd;
		bool	isCreated;
		bool	pass;
		int		nbmsg;

	public:

		Client(std::string nick, std::string user, int fd);
		~Client(void);

		//getters
		const std::string	&getNickname(void) const;
		const std::string	&getUsername(void) const;
		const bool			&getCreated(void) const;
		const int			&getFd(void) const;
		const bool		&getPass(void) const;
		const int		&getNbmsg(void) const;

		//setters
		void				setCreatedtoTrue(void);
		void				setPasstoTrue(void);
		void				setNbmsgplusone(void);
		void				setNickname(const std::string &nick);
		void				setUsername(const std::string &user);
};
