/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lseiberr <lseiberr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:28:44 by lseiberr          #+#    #+#             */
/*   Updated: 2024/01/23 13:53:06 by lseiberr         ###   ########.fr       */
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
	this->pe = getprotobyname("tcp");
	if (!pe)
		throw (NotGoodProtocolException());
	this->fdserv = socket(PF_INET, SOCK_STREAM, this->pe->p_proto);
	if (fdserv == -1)
		throw (NotGoodFdServException());
	this->sin.sin_family = AF_INET;
	this->sin.sin_addr.s_addr = INADDR_ANY;
	this->sin.sin_port = htons(this->port);
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

//getter

int Server::getPort(void)const
{
	return this->port;
}

std::string Server::getPassword(void)const
{
	return this->password;
}

int			Server::getFdserv(void)const
{
	return this->fdserv;
}

struct	protoent	Server::getProto(void)const
{
	return *this->pe;
}

struct sockaddr_in Server::getSin(void)const
{
	return this->sin;
}

//setter

void	Server::setPort(int newPort)
{
	this->port = newPort;
}

void	Server::setPassword(std::string newPassword)
{
	this->password = newPassword;
}

//exception

const char *Server::NotGoodProtocolException::what() const throw()
{
	return ("Protocol not found");
}

const char *Server::NotGoodFdServException::what() const throw()
{
	return ("FD serv error");
}
