#pragma once

enum share_memory_environment_type
{
	share_memory_environment_invalid,
	share_memory_environment_client,
	share_memory_environment_server,
};

bool share_memory_init_environment(share_memory_environment_type);
bool share_memory_environment_is_server();
bool share_memory_environment_is_client();

