#include <iostream>
#include <sstream>
#include <thread>
#include "dbserver.h"

void console()
{
	while (true)
	{
		char buff[512] = {};
		memset(buff, 0, sizeof(buff));
		std::cin.getline(buff, 512);

		std::string s(buff);
		std::istringstream ss(s);

		std::string cmd;
		ss >> cmd;

		if (cmd == "exit")
		{
			dbserver::release();
			std::cout << "bye bye" << std::endl;
			break;
		}
		else
		{
			std::cout << "invalid cmd" << std::endl;
		}
	}
}

int main()
{
	dbserver* server = dbserver::instance();
	server->init();
	server->start();

	std::thread t(console);
	t.detach();

	server->term();
	dbserver::release();
	std::cout << "game over" << std::endl;
	return 0;
}
