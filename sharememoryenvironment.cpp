#include "sharememoryenvironment.h"

static share_memory_environment_type share_memory_environment = share_memory_environment_invalid;

bool share_memory_init_environment(share_memory_environment_type type)
{
	if (share_memory_environment == share_memory_environment_invalid)
	{
		share_memory_environment = type;
		return true;
	}
	return false;
}

bool share_memory_environment_is_server()
{
	return share_memory_environment == share_memory_environment_server;
}

bool share_memory_environment_is_client()
{
	return share_memory_environment == share_memory_environment_client;
}
