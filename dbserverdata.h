#pragma once

#include <string>

struct dbserver_data
{
	std::string _dbip;
	std::string _dbname;
	std::string _username;
	std::string _userpasswd;

	unsigned long _humankey;
	int _humancount;
	int _humansaveinterval;
	unsigned long _guildkey;
	int _guildcount;
	int _guildsaveinterval;
};
