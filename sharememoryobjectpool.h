#pragma once

#include <sys/types.h>
#include "fifo.h"
#include "sharememory.h"

template <typename T>
class share_memory_object_pool
{
public:
	share_memory_object_pool() : _capacity(0), _freelist(nullptr) {}
	share_memory_object_pool(const share_memory_object_pool&) = delete;
	share_memory_object_pool& operator=(const share_memory_object_pool&) = delete;
	~share_memory_object_pool() {}

	T& operator[](unsigned int index)
	{
		T* ptr = (T*)_sm.memory();
		assert(index < _capacity);
		return ptr[index];
	}

	bool init(key_t key, unsigned int capacity)
	{
		_capacity = capacity;
		if (!_sm.init(key, sizeof(T) * _capacity))
			return false;

		_freelist = new fifo<void*>(_capacity);
		T* ptr = (T*)_sm.memory();
		for (unsigned i = 0; i < _capacity; ++i)
			_freelist->push(ptr++);

		return true;
	}

	bool term()
	{
		_freelist->clear();
		delete _freelist;
		_freelist = nullptr;

		_sm.term();

		return true;
	}

	bool empty() const
	{
		return _freelist->empty();
	}

	T* new_object()
	{
		if (empty()) return nullptr;
		void* memory = _freelist->pop();
		return new (memory) T;
	}

	void delete_object(T* o)
	{
		if (o == nullptr) return;
		o->~T();
		_freelist->push(o);
	}
private:
	unsigned int _capacity;
	fifo<void*> *_freelist;
	share_memory _sm;
};
