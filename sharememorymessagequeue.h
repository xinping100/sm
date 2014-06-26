#pragma once

#include <vector>
#include <string.h>
#include "sharememory.h"
#include "sharememorymutex.h"

enum message_type
{
	message_begin = 1,
	message_new_object = message_begin,
	message_delete_object,
	message_end,
};

struct message
{
	int type;

	struct new_object_t
	{
		int key;
		int obj_pool_id;
	};

	struct delete_object_t
	{
		int key;
		int obj_pool_id;
	};

	union
	{
		new_object_t new_object;
		delete_object_t delete_object;
	} u;
	message() { memset(this, 0, sizeof(*this)); }
};

struct message_queue_data_struct
{
	share_memory_mutex mutex;	
	unsigned int n;
	message q[1024];

	void init() { mutex.init(); n = 0; }
	void term() { mutex.term(); }
	void lock() { mutex.lock(); }
	void unlock() { mutex.unlock(); }
};

class share_memory_message_queue
{
public:
	share_memory_message_queue();
	share_memory_message_queue(const share_memory_message_queue&) = delete;
	share_memory_message_queue& operator=(const share_memory_message_queue&) = delete;
	~share_memory_message_queue();

	bool init();
	bool term();

	bool add_message(const message& m);
	unsigned int get_message_queue(std::vector<message>& v);
private:
	share_memory _sm;
	message_queue_data_struct *_ds;
};

