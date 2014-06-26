#include <map>
#include <fstream>
#include <thread>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include "sharememoryclient.h"

share_memory_client client;

bool loop = true;

class game_player
{
public:
	game_player()
	{
		_smo = client.new_object<smo_player>(SM_KEY_PLAYER);
	}

	~game_player()
	{
		client.delete_object(SM_KEY_PLAYER, _smo);
	}

	smo_player* _smo;
	smo_player _db;
};

std::map<int, game_player*> playermap;

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
			std::cout << "bye bye" << std::endl;
			loop = false;
			break;
		}
		else if (cmd == "login")
		{
			game_player* gp = new game_player;

			char id[16] = {};
			ss >> id;
			gp->_db.id = atoi(id);

			std::ifstream f;
			f.open(id, std::ios_base::binary | std::ios_base::in);
			if (f)
			{
				//f >> gp->_db.level >> gp->_db.money;
				f.read((char*)&gp->_db.id, sizeof(gp->_db.id));
				f.read((char*)&gp->_db.level, sizeof(gp->_db.level));
				f.read((char*)&gp->_db.money, sizeof(gp->_db.money));
				assert(atoi(id) == gp->_db.id);
			}

			playermap[gp->_db.id] = gp;

			std::cout << "login done" 
				<< ",id=" << gp->_db.id 
				<< ",level=" << gp->_db.level 
				<< ",money=" << gp->_db.money 
				<< std::endl;
		}
		else if (cmd == "logout")
		{
			int id;
			ss >> id;
			auto a = playermap.find(id);
			if (a != playermap.end())
			{
				delete a->second;
				playermap.erase(a);
				std::cout << "logout done" << std::endl;
			}
		}
		else if (cmd == "levelup")
		{
			int id;
			ss >> id;
			auto a = playermap.find(id);
			if (a != playermap.end())
			{
				a->second->_db.level++;
				std::cout << "levelup to:" << a->second->_db.level << std::endl;
			}
		}
		else if (cmd == "setmoney")
		{
			int id;
			ss >> id;
			auto a = playermap.find(id);
			if (a != playermap.end())
			{
				ss >> a->second->_db.money;
				std::cout << "set money to:" << a->second->_db.money << std::endl;
			}
		}
		else if (cmd == "list")
		{
			std::cout << "list player begin" << std::endl;

			for (auto a = playermap.begin(); a != playermap.end(); ++a)
			{
				game_player* gp = a->second;
				std::cout << "roleid:" << gp->_db.id << ", level:" << gp->_db.level << ", money:" << gp->_db.money << std::endl;
			}

			std::cout << "list player end" << std::endl;
		}
		else
		{
			std::cout << "invalid cmd" << std::endl;
		}
	}
}

int main()
{
	client.init();

	std::thread t(console);
	t.detach();

	while (loop)
	{
		sleep(1);
		for (auto a = playermap.begin(); a != playermap.end(); ++a)
		{
			game_player* gp = a->second;
			gp->_smo->lock();
			gp->_smo->id = gp->_db.id;
			gp->_smo->level = gp->_db.level;
			gp->_smo->money = gp->_db.money;
			gp->_smo->unlock();
		}
		//std::cout << "loop..." << std::endl;
	}

	client.term();

	return 0;
}
