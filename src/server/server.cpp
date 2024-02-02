/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbaduel <hbaduel@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:28:44 by lseiberr          #+#    #+#             */
/*   Updated: 2024/02/02 16:21:33 by hbaduel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/server/server.hpp"

Server::Server()
{

}

Server::Server(std::string newPassword, int newPort)
{
	this->password = newPassword;
	this->port = newPort;
	this->pe = getprotobyname("tcp");
	if (!pe)
		throw (NotGoodProtocolException());
	this->fdserv = socket(PF_INET, SOCK_STREAM, this->pe->p_proto);
	if (fdserv == -1)
		throw (NotGoodFdServException());
	this->sin.sin_family = AF_INET;
	this->sin.sin_addr.s_addr = INADDR_ANY;
	this->sin.sin_port = htons(this->port);
	this->fdclient.clear();
	csin_len = sizeof(csin);
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

const char *Server::ErrorBindageException::what() const throw()
{
	return ("Error bindage adress local");
}

const char *Server::ErrorListenException::what() const throw()
{
	return ("Error listenting fd");
}

const char *Server::FdClientException::what() const throw()
{
	return ("FD Cannot open");
}

int	Server::check_fd_client()
{
	std::vector<int>::iterator it = this->fdclient.begin();
	while (it != this->fdclient.end())
	{
		if (*it == -1)
			return -1;
		it++;
	}
	return (0);
}

void	Server::init_server(void)
{
	int bd = bind(this->fdserv, (struct sockaddr*)&this->sin, sizeof(this->sin));
	if (bd == -1)
		throw(ErrorBindageException());
	int ls = listen(this->fdserv, backlog);
	if (ls == -1)
		throw(ErrorListenException());
	while (1)
	{

		this->fdclient.push_back(accept(this->fdserv, (struct sockaddr*)&this->csin, &this->csin_len));
		inet_ntoa(this->csin.sin_addr);
		(void)ntohs(this->csin.sin_port);
		if (check_fd_client() == -1)
			throw (FdClientException());
	}
}
