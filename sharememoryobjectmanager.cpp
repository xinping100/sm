#include "sharememoryobjectmanager.h"
#include "sharememoryobjectpool.h"
#include <iostream>

share_memory_object_manager::share_memory_object_manager()
	: _pool(nullptr)
{
}

share_memory_object_manager::~share_memory_object_manager()
{
}

void share_memory_object_manager::init(share_memory_object_pool* pool)
{
	_pool = pool;
}

void share_memory_object_manager::term()
{
}

bool share_memory_object_manager::add(share_memory_object* o)
{
	_objectset.insert(o);
	return true;
}

bool share_memory_object_manager::del(share_memory_object* o)
{
	_objectset.erase(o);
	return true;
}

share_memory_message_manager::share_memory_message_manager()
	: _mutex(nullptr), _msgcount(nullptr)
{
}

void share_memory_message_manager::init(share_memory_object_pool* pool)
{
	share_memory_object_manager::init(pool);

	char* ptr = (char*)_pool->extra_memory();
	_mutex = new (ptr) share_memory_mutex;
	_mutex->init();

	ptr += sizeof(*_mutex);
	_msgcount = (unsigned int*)(ptr);
	*_msgcount = 0;
}

void share_memory_message_manager::term()
{
	_mutex->term();
	_mutex->~share_memory_mutex();
}

void share_memory_message_manager::tick()
{
	_mutex->lock();

	share_memory_message* p = (share_memory_message*)_pool->object_memory();

	for (unsigned int i = 0; i < *_msgcount; ++i)
		handle_message(p + i);

	*_msgcount = 0;

	_mutex->unlock();
}

void share_memory_message_manager::handle_message(share_memory_message* msg)
{
	switch (msg->type)
	{
		case smmt_new_object:	
			{
				int key = msg->new_object.key;
				int obj_pool_id = msg->new_object.obj_pool_id;
				std::cout << "new obj: key:" << key << ", obj_pool_id:" << obj_pool_id << std::endl;
			}
			break;
		case smmt_delete_object:
			{
				int key = msg->del_object.key;
				int obj_pool_id = msg->del_object.obj_pool_id;
				std::cout << "del obj: key:" << key << ", obj_pool_id:" << obj_pool_id << std::endl;
			}
			break;
		default:
			break;
	}
}

void share_memory_gameplayer_manager::tick()
{
}
