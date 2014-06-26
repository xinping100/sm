#pragma once

#include "sharememorymutex.h"

//smo == share memory object

struct smo_player
{
	int id;
	int level;
	long money;
	share_memory_mutex mutex;

	smo_player() : id(0), level(0), money(0) {}
	void init() { mutex.init(); }
	void term() { mutex.term(); }
	void lock() { mutex.lock(); }
	void unlock() { mutex.unlock(); }
};
