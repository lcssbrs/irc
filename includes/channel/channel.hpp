#pragma once

#include <iostream>
#include <list>
#include "../client/client.hpp"

class Client;

class Channel {
	public:
		Channel(std::string &name, Client *creator);
		~Channel(void);
		void	kick(std::string &name);
		void	invite(std::string &name);
		void	topic(void);
		void	topic(std::string &topic);
		void	mode(std::string &option, std::string &arg);

	private:
		std::list<Client *>	_operators;
		std::list<Client *>	_regulars;
		std::string	_name;
		bool	_inviteOnly;
		bool	_restrictTopic;
		std::string	_topic;
		bool	_passwordUse;
		std::string	_password;
		bool	_limitUser;
		int		_nUser;

};
