/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lseiberr <lseiberr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:28:44 by lseiberr          #+#    #+#             */
/*   Updated: 2024/01/23 09:25:39 by lseiberr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/server/server.hpp"

Server::Server()
{

}

Server::Server(std::string newPassword, std::string newPort)
{
	this->password = newPassword;
	this->port = stoi(newPort);
}

Server::Server(const Server & cpy)
{
	*this = cpy;
}

Server & Server::operator=(const Server & ope)
{
	(void)ope;
	return *this;
}

Server::~Server()
{

}
