/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lseiberr <lseiberr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 16:27:33 by lseiberr          #+#    #+#             */
/*   Updated: 2024/01/23 13:54:10 by lseiberr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#define _OE_SOCKETS
#include <netdb.h>
#include <stdlib.h>
#include <exception>
#include <netinet/in.h>
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

	private:
		std::string 		password;
		int					port;
		struct	protoent	*pe;
		struct sockaddr_in	sin;
		int					fdserv;

	protected:
};

#endif
