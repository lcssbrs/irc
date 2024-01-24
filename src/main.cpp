/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lseiberr <lseiberr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:56:21 by lseiberr          #+#    #+#             */
/*   Updated: 2024/01/24 09:50:37 by lseiberr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <iomanip>
#include "../includes/irc.hpp"

int checkarg(char **ag, int ac)
{
	if ( ac != 3 || !atoi(ag[1]) || atoi(ag[1]) <= 0 || !ag[2])
	{
		std::cerr << "This program has to be used as : [NAME OF PROGRAM] [PORT] [PASSWORD]\n";
		return -1;
	}
	return (0);
}

int	try_init_server_class(char **ag)
{
	try
	{
		Server server((std::string)ag[2], (std::string)ag[1]);
	}
	catch(Server::NotGoodProtocolException & e)
	{
		std::cerr << e.what() << '\n';
		return -1;
	}
	catch(Server::NotGoodFdServException & e)
	{
		std::cerr << e.what() << '\n';
		return -1;
	}
	return (0);
}

int main(int ac, char **ag)
{
	if (checkarg(ag, ac) == -1 || try_init_server_class(ag) == -1)
		return (-1);
	Server server((std::string)ag[2], (std::string)ag[1]);
	std::cout << server.getProto().p_name << std::endl;
	try
	{
		server.init_server();
	}
	catch (Server::ErrorBindageException & e)
	{
		std::cerr << e.what() << "\n";
		return -1;
	}
	catch (Server::ErrorListenException & e)
	{
		std::cerr << e.what() << "\n";
		return -1;
	}
	catch (Server::FdClientException & e)
	{
		std::cerr << e.what() << "\n";
		return -1;
	}
}
