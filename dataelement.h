#pragma once

class data_element
{
public:
	virtual ~data_element() {}
	virtual void* data() = 0;
	virtual unsigned int length() = 0;
};

template <typename T>
class data_buildin : public data_element
{
public:
	void* data() { return &_data; }
	unsigned int length() { return sizeof(_data); }
private:
	T _data;
};

typedef data_buildin<char> data_char;
typedef data_buildin<unsigned char> data_uchar;
typedef data_buildin<short> data_short;
typedef data_buildin<unsigned short> data_ushort;
typedef data_buildin<int> data_int;
typedef data_buildin<unsigned int> data_uint;
typedef data_buildin<long> data_long;
typedef data_buildin<unsigned long> data_ulong;
typedef data_buildin<float> data_float;
typedef data_buildin<double> data_double;

template <int N>
class data_bytes : public data_element
{
	typedef unsigned char byte_t;
public:
	void* data() { return _data; }
	unsigned int length() { return sizeof(_data); }
private:
	byte_t _data[N];
};
