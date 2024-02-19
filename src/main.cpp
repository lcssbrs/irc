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

bool	*isExit(void)
{
	static bool ex = false;
	return (&ex);
}

void handler_function(int sig)
{
	(void)sig;
	std::cerr << "Server has been stopped\n";
	return ;
}

int	main(int ac, char **ag)
{
	checkarg(ag, ac);
	Server serv(atoi(ag[1]), ag[2]);
	signal(SIGINT, handler_function);
	serv.manage_server();
	return (0);
}
