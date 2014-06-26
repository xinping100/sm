#include <string.h>
#include <iostream>
#include <assert.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include "sharememory.h"
#include "sharememoryenvironment.h"

share_memory::share_memory()
	: _key(0), _id(-1), _memory(nullptr), _size(0)
{
}

share_memory::~share_memory()
{
	assert(_id == -1 && _memory == nullptr);
}

bool share_memory::init(key_t key, size_t size)
{
	_key = key;
	_size = size;

	if (share_memory_environment_is_server())
		_id = shmget(_key, _size, IPC_CREAT|IPC_EXCL|S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
	else if (share_memory_environment_is_client())
		_id = shmget(_key, 0, 0);
	else
	{
		std::cout << "call share_memory_init_environment() first, please!" << std::endl;
		return false;
	}

	if (_id == -1)
	{
		std::cout << "share_memory::init() shmget, errstr=" << strerror(errno) << std::endl;
		return false;
	}

	_memory = shmat(_id, nullptr, 0);
	if (_memory == (void*)-1)
	{
		std::cout << "share_memory::init() shmat, errstr=" << strerror(errno) << std::endl;
		_memory = nullptr;
		return false;
	}

	return true;
}

bool share_memory::term()
{
	if (_id == -1 || _memory == nullptr)
		return false;

	if (shmdt(_memory))
	{
		std::cout << "share_memory::term() shmdt, errstr=" << strerror(errno) << std::endl;
		return false;
	}

	if (share_memory_environment_is_server())
	{
		if (shmctl(_id, IPC_RMID, nullptr))
		{
			std::cout << "share_memory::term() shmctl IPC_RMID, errstr=" << strerror(errno) << std::endl;
			return false;
		}
	}

	_key = 0;
	_id = -1;
	_memory = nullptr;
	_size = 0;

	return true;
}

