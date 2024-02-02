#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#define _OE_SOCKETS
#include <netdb.h>
#include <vector>
#include <stdlib.h>
#include <exception>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <list>

#include "Channel.hpp"

#define backlog 42

class Channel;

class Server
{
	public:
	//Canonic class form
		Server();
		~Server();

	//constructor by default using port and password
		Server(std::string newPassword, int newPort);

	//getter
		int					getPort(void)const;
		std::string			getPassword(void)const;
		int					getFdserv(void)const;
		struct sockaddr_in	getSin(void)const;
		struct	protoent	getProto(void)const;

	//exception
		class NotGoodProtocolException: public std::exception
		{
			public:
				const char *what() const throw();
		};

		class NotGoodFdServException: public std::exception
		{
			public:
				const char *what() const throw();
		};

		class ErrorBindageException: public std::exception
		{
			public:
				const char *what() const throw();
		};

		class ErrorListenException: public std::exception
		{
			public:
				const char *what() const throw();
		};

		class FdClientException: public std::exception
		{
			public:
				const char *what() const throw();
		};

	//server utils

		void	init_server(void);
		int		check_fd_client();

	private:
		std::string 		password;
		int					port;
		struct	protoent	*pe;
		struct sockaddr_in	sin;
		int					fdserv;
		std::vector<int>	fdclient;
		struct sockaddr_in	csin;
		socklen_t			csin_len;

		std::list<Channel *> channels;


	protected:
};

#endif
