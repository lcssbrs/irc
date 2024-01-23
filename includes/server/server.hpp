/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lseiberr <lseiberr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 16:27:33 by lseiberr          #+#    #+#             */
/*   Updated: 2024/01/23 09:25:22 by lseiberr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#define _OE_SOCKETS
#include <netdb.h>
#include <stdlib.h>

class Server
{
	public:
		Server();
		Server(std::string newPassword, std::string newPort);
		Server(const Server &cpy);
		Server & operator=(const Server & ope);
		~Server();

	private:
		std::string password;
		int			port;

	protected:
};

#endif
