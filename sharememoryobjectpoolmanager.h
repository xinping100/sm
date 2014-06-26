#pragma once

#include "sharememorykey.h"
#include "sharememoryobjectpool.h"

class share_memory_object_pool_manager
{
public:
	share_memory_object_pool_manager();
	share_memory_object_pool_manager(const share_memory_object_pool_manager&) = delete;
	share_memory_object_pool_manager& operator=(const share_memory_object_pool_manager&) = delete;
	~share_memory_object_pool_manager();

	bool init();
	bool term();

	share_memory_object_pool* get(SM_KEY key);
private:
	share_memory_object_pool* _poollist;
};
