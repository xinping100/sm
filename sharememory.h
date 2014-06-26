#pragma once

#include <sys/types.h>

class share_memory
{
public:
	share_memory();
	share_memory(const share_memory&) = delete;
	share_memory& operator=(const share_memory&) = delete;
	~share_memory();

	bool init(key_t key, size_t size);
	bool term();

	void* memory() { return _memory; }
private:
	key_t _key;
	int _id;
	void* _memory;
	size_t _size;
};


