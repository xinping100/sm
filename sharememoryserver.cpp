#include <unistd.h>
#include <iostream>
#include "sharememorykey.h"
#include "sharememoryserver.h"
#include "sharememorylogic.h"
#include "sharememoryenvironment.h"

share_memory_server::share_memory_server()
	: _loop(true)
{
}

share_memory_server::~share_memory_server()
{
}

bool share_memory_server::init()
{
	share_memory_init_environment(share_memory_environment_server);

	_msgqueue.init();
	_playermanager.init(SM_KEY_PLAYER, 1024);
	return true;
}

bool share_memory_server::loop()
{
	while (_loop)
	{
		std::vector<message> v;
		unsigned int n = _msgqueue.get_message_queue(v);
		for (unsigned int i = 0; i < n; ++i)
		{
			message& msg = v[i];
			switch (msg.type)
			{
				case message_new_object:
					on_new_object(&msg.u.new_object);
					break;
				case message_delete_object:
					on_delete_object(&msg.u.delete_object);
					break;
				default:
					break;
			}
		}

		_playermanager.for_each(share_memmory_logic);

		usleep(1000000);
	}
	return true;
}

bool share_memory_server::term()
{
	_playermanager.term();
	_msgqueue.term();
	return true;
}

bool share_memory_server::on_new_object(message::new_object_t* arg)
{
	std::cout << "on_new_object() key=" << arg->key << ", obj pool id=" << arg->obj_pool_id << std::endl;

	switch (arg->key)
	{
		case SM_KEY_MESSAGE_Q:
			assert(false);
			break;
		case SM_KEY_PLAYER:
			_playermanager.add(arg->obj_pool_id);
			break;
		default:
			break;
	}

	return true;
}

bool share_memory_server::on_delete_object(message::delete_object_t* arg)
{
	std::cout << "on_delete_object() key=" << arg->key << ", obj pool id=" << arg->obj_pool_id << std::endl;

	switch (arg->key)
	{
		case SM_KEY_MESSAGE_Q:
			assert(false);
			break;
		case SM_KEY_PLAYER:
			_playermanager.del(arg->obj_pool_id);
			break;
		default:
			break;
	}

	return true;
}

