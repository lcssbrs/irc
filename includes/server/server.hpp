#pragma once

#include <iostream>
#include <poll.h>
#include <map>
#include <list>
#include "../client/client.hpp"
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <exception>
#include "../client/client.hpp"
#include "../channel/channel.hpp"

#define backlog 42


class Channel;
class Client;

class Server
{
	private:
		int fd_server;
		std::map<int, Client *> clients;
		std::map<std::string, Channel *> channels;
		std::list<struct pollfd> fds;
		int port;

	public:
		Server(int port);
		~Server();

		void init_server();
		void manage_loop();
		int manage_server();
		void create_client(std::string & name);
		void create_channel(std::string & name);
		void remove_client_from_channel(Client * kick);

		class BindException: public std::exception
		{
			public:
				const char *what() const throw();
		};

		class ListenException: public std::exception
		{
			public:
				const char *what() const throw();
		};

		class SocketException: public std::exception
		{
			public:
				const char *what() const throw();
		};

		class TCPException: public std::exception
		{
			public:
				const char *what() const throw();
		};
};
