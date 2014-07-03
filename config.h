#pragma once

#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif
#include <stdint.h>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <assert.h>
#include <string.h>

int b_compare(const void*, const void*);
bool get_line(std::istream& is, std::string& line);
std::string trim(const std::string&);
const char* const seperator = " ;,:|";

bool convert(const std::string& s, int& out);
bool convert(const std::string& s, int64_t& out);
bool convert(const std::string& s, double& out);
bool convert(const std::string& s, float& out) ;
bool convert(const std::string& s, std::string& out);

class config
{
public:
	typedef std::string					key_t;
	typedef std::string					value_t;
	typedef std::string::size_type		size_type_t;
	typedef std::map<key_t, value_t>	map_t;

	config(const std::string& file);       
	config(const char* file=0);       

	bool load(const char* file);

	template <typename UnaryFunction>
	void for_each(UnaryFunction uf) const
	{
		for (auto it = _map.begin(); it != _map.end(); ++it)
			uf(*it);
	}

	value_t operator[] (const key_t& key) const;
	int read_int(const key_t& key) const;
	int read_long(const key_t& key) const;
	double read_double(const key_t& key) const;
	bool read_vector3f(const key_t& key, float& x, float& y, float& z) const;
	float read_float(const key_t& key) const ;
	std::string read_string(const key_t& key) const;
	int read_time(const key_t& key) const;
	bool read_time(const key_t& key, int& hour, int& minute, int& second) const;

	template <typename T>
	int read_array(const key_t& key, std::vector<T>& v)
	{
		value_t val = read_string(key);
		if (val.empty()) return -1;

		size_t beg = 0, pos = 0;
		size_t len = val.length();
		while (beg < len)
		{
			pos = val.find_first_of(seperator, beg);
			std::string sub = val.substr(beg, (pos!=std::string::npos ? pos-beg : std::string::npos));
			T t;
			if (!convert(trim(sub), t)) 
				return -1;
			v.push_back(t);
			if (pos == std::string::npos) 
				break;
			beg = pos+1;
		}
		return v.size();
	}

	template <typename T>
	std::vector<T> read_vector(const key_t& key) const
	{
		std::vector<T> v;
		value_t val = read_string(key);
		if (!val.empty())
		{
			size_t beg = 0, pos = 0;
			size_t len = val.length();
			while (beg < len)
			{
				pos = val.find_first_of(seperator, beg);
				if (pos == beg)
				{
					beg += 1;
					continue;
				}
				std::string sub = val.substr(beg, (pos!=std::string::npos ? pos-beg : std::string::npos));
				T t;
				if (!convert(sub, t)) 
					break;
				v.push_back(t);
				if (pos == std::string::npos) 
					break;
				beg = pos+1;
			}
		}
		return v;
	}

	void dump() const;
private:
	void clear() { _map.clear(); }

	map_t _map;
};
