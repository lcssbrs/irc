/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lseiberr <lseiberr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:56:21 by lseiberr          #+#    #+#             */
/*   Updated: 2024/01/23 09:26:34 by lseiberr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../includes/irc.hpp"

int checkarg(char **ag)
{
	if (!atoi(ag[1]) || atoi(ag[1]) <= 0)
		return -1;
	return (0);
}

int main(int ac, char **ag)
{
	(void)ag;
	if (ac != 3)
	{
		std::cerr << "This program has to be used as : [NAME OF PROGRAM] [PORT] [PASSWORD]\n";
		return (-1);
	}
	if (checkarg(ag) == -1)
	{
		std::cerr << "Please enter valid args\n";
		return (-1);
	}
}
