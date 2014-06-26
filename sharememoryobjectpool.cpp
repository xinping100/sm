#include "sharememoryobjectpool.h"

share_memory_object_pool::share_memory_object_pool()
	: _objectsize(0), _capacity(0), _objectmem(nullptr), _extramem(nullptr), _freelist(nullptr), _sm(nullptr)
{
}

share_memory_object_pool::~share_memory_object_pool()
{
	delete _freelist;
	delete _sm;
}

bool share_memory_object_pool::init(key_t key, unsigned int objectsize, unsigned int capacity, unsigned int extrasize)
{
	_objectsize = objectsize;
	_capacity = capacity;

	_sm = new share_memory(key, _objectsize * _capacity + extrasize);
	if (!_sm->create())
	{
		delete _sm;
		_sm = nullptr;
		return false;
	}

	_objectmem = _sm->memory();

	char* ptr = (char*)_objectmem;

	_freelist = new fifo<void*>(_capacity);
	for (unsigned i = 0; i < _capacity; ++i, ptr += objectsize)
		_freelist->push(ptr);

	if (extrasize)
		_extramem = ptr;

	return true;
}

bool share_memory_object_pool::term()
{
	_sm->close();
	_objectmem = nullptr;
	_extramem = nullptr;
	_freelist->clear();
	return true;
}

template <typename OBJECT>
OBJECT* share_memory_object_pool::new_object()
{
	if (empty()) return nullptr;
	void* memory = _freelist->pop();
	return new (memory) OBJECT;
}

template <typename OBJECT>
void share_memory_object_pool::delete_object(OBJECT* o)
{
	if (o == nullptr) return;
	o->~OBJECT();
	_freelist->push(o);
}
