#pragma once

#include "sharememoryobjectmanager.h"
#include "sharememorymessagequeue.h"
#include "sharememoryobject.h"

class share_memory_server
{
public:
	share_memory_server();
	share_memory_server(const share_memory_server&) = delete;
	share_memory_server& operator=(const share_memory_server&) = delete;
	~share_memory_server();

	bool init();
	bool loop();
	bool term();

	bool on_new_object(message::new_object_t*);
	bool on_delete_object(message::delete_object_t*);

	void exit() { _loop = false; }
private:
	bool _loop;
	share_memory_message_queue _msgqueue;
	share_memory_object_manager<smo_player> _playermanager;
};
