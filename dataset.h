#pragma once

#include <vector>
#include "dataelement.h"

enum
{
	KEY_BEGIN = 1,
	KEY_GAME_PLAYER = KEY_BEGIN,
	KEY_END,
};

template <int KEY>
class data_set
{
public:
	data_set();
	~data_set()
	{
		for (data_element*& e : _elements)
			delete e;
	}

	unsigned int length() const
	{
		unsigned int sum = 0;
		for (data_element*& e : _elements)
			sum += e->length();
		return sum;
	}
private:
	std::vector<data_element*> _elements;
};

template <>
data_set<KEY_GAME_PLAYER>::data_set()
{
	_elements.resize(3);
	_elements[0] = new data_int;
	_elements[1] = new data_float;
	_elements[2] = new data_bytes<6>;
}
