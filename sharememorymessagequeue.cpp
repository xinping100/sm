#include <assert.h>
#include <new>
#include "sharememorymessagequeue.h"
#include "sharememorykey.h"

share_memory_message_queue::share_memory_message_queue()
	: _ds(nullptr)
{}

share_memory_message_queue::~share_memory_message_queue()
{
	assert(_ds == nullptr);
}

bool share_memory_message_queue::init()
{
	if (!_sm.init(SM_KEY_MESSAGE_Q, sizeof(*_ds)))
		return false;

	_ds = new (_sm.memory()) message_queue_data_struct;
	_ds->init();

	return true;
}

bool share_memory_message_queue::term()
{
	_ds->term();
	_ds->~message_queue_data_struct();
	_ds = nullptr;

	_sm.term();

	return true;
}

bool share_memory_message_queue::add_message(const message& msg)
{
	bool ret = false;
	_ds->lock();
	if (_ds->n < 1024)
	{
		_ds->q[_ds->n++] = msg;
		ret = true;
	}
	_ds->unlock();
	return ret;
}

unsigned int share_memory_message_queue::get_message_queue(std::vector<message>& v)
{
	_ds->lock();
	unsigned int i = 0;
	for (; i < _ds->n; ++i)
		v.push_back(_ds->q[i]);
    _ds->n = 0;
	_ds->unlock();

	return i;
}
