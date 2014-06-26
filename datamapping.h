#pragma once

#include "dataset.h"

template <int KEY>
class data_mapping
{
public:
	data_mapping()
	{

	}

	void memory_copy();
private:
	void* _data_sm;
	data_set<KEY> _data_set;
};
