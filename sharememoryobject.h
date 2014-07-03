#pragma once

#include "sharememorymutex.h"
#include "tablehuman.h"

//smo == share memory object

struct smo_player
{
	table_human _tabledata;
	bool _dirty;
	share_memory_mutex mutex;

	smo_player() {}
	void init() { _dirty = false; mutex.init(); }
	void term() { mutex.term(); }
	void lock() { mutex.lock(); }
	void unlock() { mutex.unlock(); }
	void set_dirty(bool flag) { _dirty = flag; }
	bool is_dirty() { return _dirty; }
};
