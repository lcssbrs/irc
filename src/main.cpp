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

Server **isServ()
{
	static Server *serv;
	return (&serv);
}

void handler_function(int sig)
{
	if (sig != SIGINT)
		return ;
	std::cerr << "Server has been stopped\n";
	delete (*isServ());
}

void set_signal_action(void)
{
	struct sigaction	act;
	bzero(&act, sizeof(act));
	act.sa_handler = &handler_function;
	sigaction(SIGINT, &act, NULL);
}

int	main(int ac, char **ag)
{
	checkarg(ag, ac);
	*isServ() = new Server(atoi(ag[1]), ag[2]);
	set_signal_action();
	(*isServ())->manage_server();
	return (0);
}
