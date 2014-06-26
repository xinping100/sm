#pragma once

#include <pthread.h>
#include <errno.h>
#include "sharememoryenvironment.h"

class share_memory_mutex
{
public:
	share_memory_mutex() {}
	share_memory_mutex(const share_memory_mutex&) = delete;
	share_memory_mutex& operator=(const share_memory_mutex&) = delete;
	~share_memory_mutex() {}

	void init()
	{
		if (share_memory_environment_is_server())
		{
			pthread_mutexattr_t attr;
			pthread_mutexattr_init(&attr);
			pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
			pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
			pthread_mutex_init(&_mutex, &attr);
			pthread_mutexattr_destroy(&attr);
		}
	}

	void term()
	{
		if (share_memory_environment_is_server())
		{
			while (pthread_mutex_destroy(&_mutex) == EBUSY)
			{
				lock(); 
				unlock();
			}
		}
	}

	void lock()
	{
		pthread_mutex_lock(&_mutex);
	}

	void unlock()
	{
		pthread_mutex_unlock(&_mutex);
	}
private:
	pthread_mutex_t _mutex;
};
