#include <iostream>
#include <sstream>
#include <thread>
#include "sharememoryserver.h"

share_memory_server server;

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
			server.exit();
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
	server.init();

	std::thread t(console);
	t.detach();

	server.loop();

	server.term();

	std::cout << "game over" << std::endl;
	return 0;
}
