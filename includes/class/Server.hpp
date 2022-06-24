#ifndef SERVER_CLASS_HPP
# define SERVER_CLASS_HPP

#include <iostream>
#include <map>
#include <string>
#include <sys/types.h> // socket, bind, listen, recv, send
#include <sys/socket.h>//   "      "      "      "     "
#include <netinet/in.h>// sockaddr_in
#include <unistd.h>// fcntl
#include <fcntl.h> //   "
#include <poll.h>
#include "color.h"
#include "class/User.hpp"
#include "class/Channel.hpp"

class Server;

enum	e_state
{
	SHUTDOWN,
	RUNNING
};

enum	e_logMsg
{
	INTERNAL,
	RECEIVED,
	SENT,
};

enum	e_rplNo
{
	RPL_WELCOME = 001,
	RPL_YOUREOPER = 381,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_NICKNAMEINUSE = 433,
};

typedef bool	(Server::*t_fct)(User &user, std::string const &params);

class Server
{
private:
	// Attributes
	int												_state;
	int												_socket;

	std::string										_name;
	std::string										_password;

	std::vector<pollfd>								_pollfds;
	std::map<int, User>								_users; // int is for id/socket
	std::map<std::string, Channel>					_channels; // string is for the name's channel
	std::map<std::string const, t_fct const>		_cmds;

	static std::pair<std::string const, t_fct const> const		_lookupCmds[];
	static std::pair<enum e_logMsg const, char const *> const	_lookupLogMsgTypes[];

	// Member functions
	static void	logMsg(enum e_logMsg const type, std::string const &msg);


	bool	cmdDie(User &user, std::string const &params);
	bool	cmdJoin(User &user, std::string const &params);
	bool	cmdKick(User &user, std::string const &params);
	bool	cmdKill(User &user, std::string const &params);
	bool	cmdMsg(User &user, std::string const &params);
	bool	cmdNick(User &user, std::string const &params);
	bool	cmdOper(User &user, std::string const &params);
	bool	cmdPart(User &user, std::string const &params);
	bool	cmdPass(User &user, std::string const &params);
	bool	cmdPing(User &user, std::string const &params);
	bool	cmdQuit(User &user, std::string const &params);
	bool	cmdSet(User &user, std::string const &params);
	bool	cmdUser(User &user, std::string const &params);
	bool	reply(User const &user, std::string const &msg) const;

public:
	// Constructors
	Server( void );

	// Destructors
	virtual ~Server(void);

	// Accessors
	int	getSocket(void) const; // XXX temporary
	int	getState(void) const;

	// Member functions
	bool	addUser(void); // Save a new user in the server, calling cmdPass(), then cmdNick(), and cmdUser()
	bool	init(); // get the port, the password & the config (from config files)
	bool	judge(User &user, std::string &msg);
	bool	start(uint16_t const port); // socket() + bind() + listen() + fcntl() <-- setup non-blocking fd
	bool	stop(void); // disconnect all users + _users.clear() + close(_socket) + _socket = INVALID_SOCKET (-1)
	bool	run(); // principal loop (that call update, ect)
	bool	update(); // loop (accept() new users + setup new users) + select()/poll() + loop recv() new messages (1 per user)

	bool	welcomeDwarves(void);
	bool	listenAll(void);

	/**
	 * optionals command:
	 * 
	 * 	-ping
	 * 	-nick
	 * 	-names
	 * 	-time
	 * 	-ban
	 * 	-kickban
	 */
};

#endif