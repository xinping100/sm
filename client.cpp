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
			gp->_db._tabledata._guid_k = atoi(id);

			std::ifstream f;
			f.open(id, std::ios_base::binary | std::ios_base::in);
			if (f)
			{
				f >> gp->_db._tabledata._guid_k;
				f >> gp->_db._tabledata._name_q;
				f >> gp->_db._tabledata._level;
				f >> gp->_db._tabledata._age;
				gp->_db.set_dirty(true);
				assert(atoi(id) == gp->_db._tabledata._guid_k);
			}

			playermap[gp->_db._tabledata._guid_k] = gp;

			std::cout << "login done" 
				<< ",id=" << gp->_db._tabledata._guid_k 
				<< ",name=" << gp->_db._tabledata._name_q
				<< ",level=" << gp->_db._tabledata._level 
				<< ",age=" << gp->_db._tabledata._age 
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
				a->second->_db._tabledata._level++;
				std::cout << "levelup to:" << a->second->_db._tabledata._level << std::endl;
			}
		}
		else if (cmd == "list")
		{
			std::cout << "list player begin" << std::endl;

			for (auto a = playermap.begin(); a != playermap.end(); ++a)
			{
				game_player* gp = a->second;
				std::cout << "roleid:" << gp->_db._tabledata._guid_k << ", level:" << gp->_db._tabledata._level << ", age:" << gp->_db._tabledata._age << std::endl;
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
			if (gp->_db.is_dirty())
			{
				gp->_smo->lock();
				gp->_smo->_tabledata._guid_k = gp->_db._tabledata._guid_k;
				memset(gp->_smo->_tabledata._name_q, 0, sizeof(gp->_smo->_tabledata._name_q));
				memcpy(gp->_smo->_tabledata._name_q, gp->_db._tabledata._name_q, sizeof(gp->_db._tabledata._name_q));
				gp->_smo->_tabledata._level = gp->_db._tabledata._level;
				gp->_smo->_tabledata._age = gp->_db._tabledata._age;
				gp->_smo->set_dirty(gp->_db._dirty);
				gp->_smo->unlock();
				gp->_db.set_dirty(false);
			}
		}
	}

	client.term();

	return 0;
}
