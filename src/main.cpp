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
	checkarg(ag, ac);
	Server serv(atoi(ag[1]));
	serv.manage_server();

	// (void)ac; //main pour test channels functions sans serveur
	// (void)ag;
	// std::map<int, Client *>	clients;
	// srand(time(0));
	// char c;
	// std::string temp;
	// for (int i = 0; i < 10; i++)
	// {
	// 	c = i + '0';
	// 	temp = c;
	// 	clients[i] = new Client(temp, temp, i);
	// }

	// std::map<std::string, Channel *>	channels;
	// std::string	canal = "canal";
	// channels[canal] = new Channel(canal, clients[0]);
	// for (int i = 1; i < 10; i++)
	// 	channels[canal]->userJoin(clients[i], "");
	// channels[canal]->printClients();
	// std::cout << std::endl << "/!\\ kick test" << std::endl;
	// temp = "marge";
	// channels[canal]->kick(clients[0], temp);
	// temp = "2";
	// channels[canal]->kick(clients[1], temp);
	// channels[canal]->kick(clients[0], temp);
	// channels[canal]->printClients();
	// std::cout << std::endl << "/!\\ invite test" << std::endl;
	// channels[canal]->userJoin(channels[canal]->invite(clients[1], temp, clients), "");
	// channels[canal]->userJoin(channels[canal]->invite(clients[0], temp, clients), "");
	// channels[canal]->printClients();
	// std::cout << std::endl << "/!\\ topic test" << std::endl;
	// channels[canal]->topic();
	// temp = "test";
	// channels[canal]->topic(clients[1], temp);
	// channels[canal]->topic();
	// std::cout << std::endl << "/!\\ mode test" << std::endl;
	// temp = "i"; //i t k o l
	// std::string temp2;
	// channels[canal]->mode(clients[1], temp, temp2);
	// channels[canal]->mode(clients[0], temp, temp2);
	// temp = "t";
	// channels[canal]->mode(clients[0], temp, temp2);
	// temp = "k";
	// temp2 = "mdp";
	// channels[canal]->mode(clients[0], temp, temp2);
	// temp = "o";
	// temp2 = "marge";
	// channels[canal]->mode(clients[0], temp, temp2);
	// temp2 = "1";
	// channels[canal]->mode(clients[0], temp, temp2);
	// temp = "k";
	// temp2 = "";
	// channels[canal]->mode(clients[1], temp, temp2);
	// temp = "o";
	// temp2 = "1";
	// channels[canal]->mode(clients[0], temp, temp2);
	// temp = "k";
	// temp2 = "mdp";
	// channels[canal]->mode(clients[1], temp, temp2);
	// channels[canal]->mode(clients[0], temp, temp2);
	// temp = "l";
	// temp2 = "10";
	// channels[canal]->mode(clients[0], temp, temp2);
	// channels[canal]->printStatus();
	// std::cout << std::endl << "/!\\ topic test after restrict" << std::endl;
	// channels[canal]->topic();
	// temp = "test2";
	// channels[canal]->topic(clients[1], temp);
	// channels[canal]->topic(clients[0], temp);
	// channels[canal]->topic();



	// for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
	// 	delete it->second;
	// delete channels[canal];
	return (0);
}
