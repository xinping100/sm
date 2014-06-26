#include "sharememoryobjectpoolmanager.h"
#include "sharememoryobject.h"
#include "sharememorymutex.h"

share_memory_object_pool_manager::share_memory_object_pool_manager()
	: _poollist(nullptr)
{
}

share_memory_object_pool_manager::~share_memory_object_pool_manager()
{
}

bool share_memory_object_pool_manager::init()
{
	_poollist = new share_memory_object_pool[SM_KEY_NUM];

	unsigned int extra_len = sizeof(share_memory_mutex) + sizeof(unsigned int);
	_poollist[SM_KEY_MESSAGE].init(SM_KEY_MESSAGE, sizeof(smo_message), 64, extra_len);
	_poollist[SM_KEY_GAMEPLAYER].init(SM_KEY_GAMEPLAYER, sizeof(smo_gameplayer), 64);
	return true;
}

bool share_memory_object_pool_manager::term()
{
	_poollist[SM_KEY_MESSAGE].term();
	_poollist[SM_KEY_GAMEPLAYER].term();
	delete [] _poollist;
	_poollist = nullptr;
	return true;
}

share_memory_object_pool* share_memory_object_pool_manager::get(SM_KEY key)
{
	return &_poollist[key];
}
