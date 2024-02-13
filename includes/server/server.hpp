#pragma once

#include <iostream>
#include <poll.h>
#include <map>
#include <vector>
#include <fcntl.h>
#include "../client/client.hpp"
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <exception>
#include "../client/client.hpp"
#include "../channel/channel.hpp"
#include <unistd.h>

#define backlog 42


class Channel;
class Client;

class Server
{
	private:
		int fd_server;
		std::map<int, Client *> clients;
		std::map<std::string, Channel *> channels;
		std::vector<struct pollfd> fds;
		int port;
		std::string password;

	public:
		Server(int port, std::string pass);
		~Server();

		void init_server();
		void manage_loop();
		int manage_server();
		void create_client(std::string & buffer, Client & client, std::vector<struct pollfd> fds, int i);
		void create_channel(std::string & name, client * client);
		void remove_client_from_channel(Client * kick);
		void parsing_msg(std::string & buffer, int fd, std::vector<struct pollfd> fds, int i);
		void closeClient(Client & client, std::vector<struct pollfd> fds, int i);
		void check_command(std::string buffer, Client * client);

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
