#include "class/Server.hpp"

/**
 * @brief	Make an user leaving a channel.
 * 
 * @param	user The user that ran the command.
 * @param	params The parameters of the command.
 * 
 * @return	true if success, false otherwise.
 */
bool	Server::PART(User &user, std::string &params)
{
	std::vector<std::string>	channel_left;

	Server::logMsg(RECEIVED, "(" + Server::toString(user.getSocket()) + ") PART " + params);
	params = params.c_str() + params.find(':') + 1;
	while (params.find(',') != std::string::npos)
	{
		channel_left.push_back(params.substr(0, params.find(',')));
		params = params.c_str() + params.find(',') + 1;
	}
	channel_left.push_back(params.substr(0, params.find(',')));
	for(std::vector<std::string>::iterator ite = channel_left.begin();ite != channel_left.end();ite++)
	{
		Channel	&myChan = this->_channels[*ite];
		if (myChan.getUsers().size() == 1)
		{
			this->_channels.erase(*ite);
		}
		else
		{
			myChan.getUsers().erase(std::find(myChan.getUsers().begin(), myChan.getUsers().end(), user));
			std::string	cpy = *ite + " has left the channel";
			PRIVMSG(user, cpy);
		}
	}
	return true;
}
