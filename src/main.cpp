/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lseiberr <lseiberr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:56:21 by lseiberr          #+#    #+#             */
/*   Updated: 2024/01/23 13:55:24 by lseiberr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <iomanip>
#include "../includes/irc.hpp"

int checkarg(char **ag)
{
	if (!atoi(ag[1]) || atoi(ag[1]) <= 0 || !ag[2])
		return -1;
	return (0);
}

int main(int ac, char **ag)
{
	(void)ag;
	if (ac != 3 || checkarg(ag) == -1)
	{
		std::cerr << "This program has to be used as : [NAME OF PROGRAM] [PORT] [PASSWORD]\n";
		return (-1);
	}
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
	Server server((std::string)ag[2], (std::string)ag[1]);
	std::cout << server.getProto().p_name << std::endl;
}
