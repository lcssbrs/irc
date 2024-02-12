#include <iostream>
#include <iomanip>
#include "../includes/irc.hpp"

void checkarg(char **ag, int ac)
{
	if (ac != 3 || !atoi(ag[1]) || atoi(ag[1]) <= 0 || !ag[2])
	{
		std::cerr << "This program has to be used as : [NAME OF PROGRAM] [PORT] [PASSWORD]\n";
		exit(1);
	}
}

int	main(int ac, char **ag)
{
	// checkarg(ag, ac);
	// Server serv(atoi(ag[1]));
	// serv.manage_server();



	(void)ac; //main pour test channels functions sans serveur
	(void)ag;
	std::map<int, Client *>	clients;
	srand(time(0));
	char c;
	std::string temp;
	for (int i = 0; i < 10; i++)
	{
		c = i + '0';
		temp = c;
		clients[i] = new Client(temp, temp, i);
	}

	std::map<std::string, Channel *>	channels;
	std::string	canal = "canal";
	channels[canal] = new Channel(canal, clients[0]);
	for (int i = 1; i < 10; i++)
		channels[canal]->userJoin(clients[i]);
	channels[canal]->printClients();
	temp = "marge";
	channels[canal]->kick(clients[0], temp);
	temp = "2";
	channels[canal]->kick(clients[1], temp);
	channels[canal]->kick(clients[0], temp);
	channels[canal]->printClients();
	channels[canal]->userJoin(channels[canal]->invite(clients[1], temp, clients));
	channels[canal]->userJoin(channels[canal]->invite(clients[0], temp, clients));
	channels[canal]->printClients();
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
		delete it->second;
	delete channels[canal];
	return (0);
}
