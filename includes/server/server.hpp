#pragma once

#include <iostream>
#include <poll.h>
#include <map>
#include <unistd.h>
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
#include <ctime>

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

		void	init_server();
		void	manage_loop();
		int		manage_server();
		void	send_ping(int fd);
		void	mode_channel(std::string channel, Client * client);
		void	create_client(std::string & buffer, Client & client, int i);
		void	create_channel(std::string name, Client * client);
		void	remove_client_from_channel(Client * kick);
		void	parsing_msg(std::string & buffer, int fd, int i);
		void	closeClient(Client &client, int i);
		int		checkNickname(std::string nick, int fd);
		void	sendmessagetoclient(Client * client, std::string buffer);
		void	ft_kick(Client * client, std::string channel ,std::string name);

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
