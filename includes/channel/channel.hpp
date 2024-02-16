#pragma once

#include <iostream>
#include <map>
#include <cstdlib>
#include <unistd.h>
#include "../client/client.hpp"

class Client;

class Channel {
	public:
		Channel(std::string &name, std::string password, Client *creator);
		~Channel(void);

		//operators command
		void	kick(Client *user, std::string &name);
		Client	*invite(Client *user, std::string &name, std::map<int, Client *> &clients);
		void	topic(Client *user) const;
		void	topic(Client *user, std::string &topic);
		void	mode(Client *user, bool change, std::string &option, std::string &arg);

		void	userJoin(Client *user, std::string password);
		int		userLeave(Client *user, std::string msg);
		void	sendNewcomer(Client *user);
		void	sendMessage(Client *user, std::string msg);
		void	sendLeave(Client *user, std::string &msg);
		void	sendAll(std::string &msg);

		//getters
		const std::string	&getName(void) const;

	private:
		std::map<std::string, Client *>	_operators;
		std::map<std::string, Client *>	_regulars;
		std::string	_name;
		bool	_inviteOnly;
		bool	_restrictTopic;
		std::string	_topic;
		bool	_passwordUse;
		std::string	_password;
		bool	_limitUser;
		int		_nUser;
};
