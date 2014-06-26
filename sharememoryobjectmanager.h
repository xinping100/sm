#pragma once

#include <set>
#include <assert.h>
#include "sharememoryobjectpool.h"

template <typename T>
class share_memory_object_manager
{
public:
	share_memory_object_manager() : _pool(nullptr) {}
	share_memory_object_manager(const share_memory_object_manager&) = delete;
	share_memory_object_manager& operator=(const share_memory_object_manager&) = delete;
	~share_memory_object_manager() { assert(_pool == nullptr); }

	bool init(key_t key, unsigned int capacity)
	{
		_pool = new share_memory_object_pool<T>;
		return _pool->init(key, capacity);
	}

	bool term()
	{
		bool b = _pool->term();
		delete _pool;
		_pool = nullptr;
		return b;
	}

	template <typename FUNC>
	void for_each(FUNC func)
	{
		for (auto &a : _objectset)
			func(a);
	}

	T* new_object()
	{
		return _pool->new_object();
	}

	void delete_object(T* t)
	{
		_pool->delete_object(t);
	}

	bool add(unsigned int pool_id)
	{
		return add(&(*_pool)[pool_id]);	
	}

	bool del(unsigned int pool_id)
	{
		return del(&(*_pool)[pool_id]);	
	}

	bool add(T* t)
	{
		_objectset.insert(t);
		return true;
	}

	bool del(T* t)
	{
		_objectset.erase(t);
		return true;
	}

	unsigned int get_pool_id(void* t) const
	{
		return (T*)t - &(*_pool)[0];
	}
private:
	std::set<T*> _objectset;
	share_memory_object_pool<T> *_pool;
};
