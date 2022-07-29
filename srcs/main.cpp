#include <cerrno>
#include <cstdlib>
#include <iostream>
#include "color.h"
#include "class/Config.hpp"
#include "class/Server.hpp"

inline static bool	__getPort(std::string const str, uint16_t &port)
{
	std::string::const_iterator it;

	for (it = str.begin(); it != str.end(); ++it)
	{
		if (!isdigit(*it))
		{
			std::cerr << "error: port: wrong value\n";
			return false;
		}
	}
	port = strtol(str.c_str(), NULL, 10);
	if (errno == ERANGE)
	{
		std::cerr << "error: port: out of range\n";
		return false;
	}
	return true;
}

int	main(int const argc, char const *const *const argv)
{
	/* Server		server;
	uint16_t	port;

	if (argc != 3)
	{
		std::cerr
		<< RED "Error: Wrong usage\n"
		<< YELLOW "./ircserv <port> <password>\n"
		<< RESET;
		return EXIT_FAILURE;
	}
	if (!__getPort(argv[1], port) ||
		!server.init(argv[2]) ||
		!server.start(port) ||
		!server.run())
		return EXIT_FAILURE;
	server.stop();
	std::cout << "Project is not working Yet " RED "We are Sorry" RESET << '\n'; */
	Config	c;

	c.init("config/default.conf");
	for (std::map<std::string const, std::string>::const_iterator cit = c._lookupValues.begin(); cit != c._lookupValues.end(); cit++)
	{
		std::cout << "[" << cit->first << "] = [" << cit->second << "]\n";
	}
	
	return EXIT_SUCCESS;
}
