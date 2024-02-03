#include "../../includes/server/server.hpp"

Server::Server(int port)
{
	this->port = port;
	this->fds.clear();
}

Server::~Server()
{

}

const char *Server::BindException::what() const throw()
{
	return "Error bindage";
}

const char *Server::ListenException::what() const throw()
{
	return "Error listnening";
}

const char *Server::TCPException::what() const throw()
{
	return "Error protocol";
}

const char *Server::SocketException::what() const throw()
{
	return "Error socket server";
}

void Server::init_server()
{
	struct sockaddr_in	sin;
	struct protoent *pe = getprotobyname("tcp");
	if (pe == NULL)
		throw (TCPException());
	this->fd_server = socket(PF_INET, SOCK_STREAM, pe->p_proto);
	if (this->fd_server == -1)
		throw (SocketException());
	sin.sin_family = AF_INET; //ipv4
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(this->port);
	int yes=1;
	if (setsockopt(this->fd_server, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
	{
		std::cerr << "error reuse bind\n";
		exit(1);
	}
	if (bind(this->fd_server, (struct sockaddr*)&sin, sizeof(sin)) == -1)
		throw (BindException());
	if (listen(this->fd_server, backlog) == -1)
		throw (ListenException());
	struct pollfd pollfd;
	this->fds.push_front(pollfd);
	this->fds.front().fd = fd_server;
	this->fds.front().events = POLLIN;
}

static int get_line(int fd, std::string &line)
{
	char chr[2] = {0};
	int readed = 0;
	int total_read = 0;;
	while ((readed = recv(fd,chr, 1, 0)) > 0){
		total_read += readed;
		std::string append(chr);
		line += append;
		if (chr[0] == '\n')
			break;
		memset(chr, 0, 2);
	}
	return total_read;
}

void Server::manage_loop()
{
	struct sockaddr	sin;
	socklen_t len = sizeof(sin);
	struct pollfd pollf;
	std::string line[backlog];
	while (1)
	{
		poll(&fds.front(), fds.size(), 1);
		fds.push_back(pollf);
		this->fds.back().fd = accept(this->fd_server, &sin, &len);
		if (this->fds.back().fd == -1)
			std::cout << "fd pas valid\n";
		this->fds.back().events = POLLIN;
		if (fds.back().fd != fd_server)
		{
			std::cout << "test\n";
			std::list<pollfd>::iterator itfds = fds.begin();
			itfds++;
			while (itfds != fds.end())
			{
				std::string buff;
				int readed = get_line((*itfds).fd, buff);
				if (readed > 0)
				{
					line[(*itfds).fd] += buff;
					std::cout << line[(*itfds).fd] << std::endl;
				}
				std::cout << (*itfds).fd << std::endl;
				itfds++;
			}
		}
	}
}

int Server::manage_server()
{
	try
	{
		init_server();
	}
	catch(BindException & e)
	{
		std::cerr << e.what() << std::endl;
		exit (1);
	}
	catch(SocketException & e)
	{
		std::cerr << e.what() << std::endl;
		exit(1);
	}
	catch(TCPException & e)
	{
		std::cerr << e.what() << std::endl;
		exit(1);
	}
	catch(ListenException & e)
	{
		std::cerr << e.what() << std::endl;
		exit(1);
	}
	manage_loop();
	return (0);
}

void Server::create_client(std::string & name)
{
	(void)name;
}

void Server::create_channel(std::string & name)
{
	(void)name;
}

void Server::remove_client_from_channel(Client * kick)
{
	(void)kick;
}
