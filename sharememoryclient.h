#pragma once

#include "sharememoryobjectmanager.h"
#include "sharememorymessagequeue.h"
#include "sharememoryobject.h"
#include "sharememorykey.h"

class share_memory_client
{
public:
	share_memory_client() {}
	share_memory_client(const share_memory_client&) = delete;
	share_memory_client& operator=(const share_memory_client&) = delete;

	bool init();
	bool term();

	unsigned int get_pool_id(int key, void* ptr)
	{
		if (key == SM_KEY_PLAYER)
			return _playermanager.get_pool_id(ptr);
		return 0;
	}

	template <typename T>
	T* new_object(int key)
	{
		T* t = do_new_object<T>(key);
		if (t)
		{
			message msg;
			msg.type = message_new_object;
			msg.u.new_object.key = key;
			msg.u.new_object.obj_pool_id = get_pool_id(key, t);
			_msgqueue.add_message(msg);
		}
		return t;
	}

	template <typename T>
	void delete_object(int key, T* t)
	{
		do_delete_object<T>(key, t);

		message msg;
		msg.type = message_delete_object;
		msg.u.delete_object.key = key;
		msg.u.delete_object.obj_pool_id = get_pool_id(key, t);
		_msgqueue.add_message(msg);
	}
private:
	template <typename T>
	T* do_new_object(int key)
	{
		if (key == SM_KEY_PLAYER)
			return _playermanager.new_object();
		return nullptr;
	}

	template <typename T>
	void do_delete_object(int key, T* o)
	{
		if (key == SM_KEY_PLAYER)
			_playermanager.delete_object(o);
	}

	share_memory_message_queue _msgqueue;
	share_memory_object_manager<smo_player> _playermanager;
};

