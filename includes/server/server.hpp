/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lseiberr <lseiberr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 16:27:33 by lseiberr          #+#    #+#             */
/*   Updated: 2024/01/23 15:33:48 by lseiberr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

#define backlog 42

class Server
{
	public:
	//Canonic class form
		Server();
		Server(const Server &cpy);
		Server & operator=(const Server & ope);
		~Server();

	//constructor by default using port and password
		Server(std::string newPassword, std::string newPort);

	//getter
		int					getPort(void)const;
		std::string			getPassword(void)const;
		int					getFdserv(void)const;
		struct sockaddr_in	getSin(void)const;
		struct	protoent	getProto(void)const;

	//setter
		void	setPort(int newPort);
		void	setPassword(std::string newPassword);

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

	protected:
};

#endif
