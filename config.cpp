#include "config.h"

config::config(const std::string& file) 
{
   	if (!file.empty()) load(file.c_str()); 
}       

config::config(const char* file) 
{ 
	if (file) load(file); 
}       

bool config::load(const char* file)
{       
	//clear();支持append
	std::fstream f(file, std::ios::in);
	if (!f) return false;
	std::string line;
	while (get_line(f, line))
	{       
		if (line.empty() || line.at(0) == '#') 
			continue;
		if (std::count(line.begin(), line.end(), '=') != 1)
			continue ;
		std::string::size_type loc = line.find('=', 0);
		_map[trim(line.substr(0, loc))] = trim(line.substr(loc+1));
	}       
	return true;
}   

config::value_t config::operator[] (const key_t& key) const
{
	auto it = _map.find(key);
	return it != _map.end() ? it->second : "";
}

int config::read_int(const key_t& key) const
{
	value_t v = operator[](key);
	return v.empty() ? 0 : atoi(v.c_str());
}

int config::read_long(const key_t& key) const
{
	value_t v = operator[](key);
	return v.empty() ? 0 : atol(v.c_str());
}

double config::read_double(const key_t& key) const
{
	value_t v = operator[](key);
	return v.empty() ? 0 : atof(v.c_str());
}

bool config::read_vector3f(const key_t& key, float& x, float& y, float& z) const
{
	std::vector<float> v = read_vector<float>(key);
	if (v.size() == 3)
	{
		x = v[0]; y = v[1]; z = v[2];
		return true;
	}
	return false;
}

float config::read_float(const key_t& key) const 
{ 
	return float(read_double(key)); 
}

std::string config::read_string(const key_t& key) const 
{ 
	return operator[](key); 
}

int config::read_time(const key_t& key) const
{
	value_t s_time = operator[](key);
	if (std::count(s_time.begin(), s_time.end(), ':') != 2)
		return 0;

	size_type_t loc1 = s_time.find_first_of(':');
	size_type_t loc2 = s_time.find_last_of(':');
	int hour = atoi(s_time.substr(0, loc1).c_str());
	int minute = atoi(s_time.substr(loc1+1, loc2-loc1-1).c_str());
	int second = atoi(s_time.substr(loc2+1).c_str());

	if (hour<24 && minute<60 && second<60)
		return hour*3600 + minute*60 + second;
	return 0;
}

bool config::read_time(const key_t& key, int& hour, int& minute, int& second) const
{
	if (int time = read_time(key))
	{
		hour = time / 3600;
		minute = time % 3600 / 60;
		second = time % 3600 % 60;
		return true;
	}
	return false;
}

void config::dump() const
{
	for (auto it = _map.begin(), ite = _map.end(); it != ite; ++it)
		std::cout << it->first << "\t\t= " << it->second << std::endl; 
}

inline bool get_line(std::istream& is, std::string& line)
{
	if (!std::getline(is, line))
		return false;
#ifndef WIN32
	size_t len = line.length();
	if (len && line.at(len-1) == '\r')
		line = line.substr(0, len-1);
#endif
	return true;
}

inline std::string trim(const std::string& s)
{
	std::string space = " \t;";
	std::string::size_type first = s.find_first_not_of(space);
	std::string::size_type last = s.find_last_not_of(space);
	return s.substr(first, last-first+1);
}

/*bool convert(const std::string& s, int& out) 
{ 
	int64_t i64 = 0;
	bool negative = false;
	for (auto it=s.begin(); it!=s.end(); ++it)
	{
		if (*it == '-')
		{
			if (it != s.begin())
				return false;
			negative = true;
		}
		else
		{
			if (!isdigit(*it))
				return false;

			i64 *= 10;
			i64 += *it - '0';

			if (negative && -i64 < INT32_MIN)
				return false;

			if (!negative && i64 > INT32_MAX)
				return false;
		}
	}
	out = int(negative ? -i64 : i64);
	return true;
}

bool convert(const std::string& s, int64_t& out) 
{ 
	int64_t i64 = 0;
	bool negative = false;
	for (auto it = s.begin(); it != s.end(); ++it)
	{
		if (*it == '-')
		{
			if (it != s.begin())
				return false;
			negative = true;
		}
		else
		{
			if (!isdigit(*it))
				return false;

			char val = (*it) - '0';
			if (negative)
			{
				int64_t test = INT64_MIN;
				for (int i=0; i<10; ++i)
					if ((test += i64) > 0)
						return false;
				if ((test += val) > 0)
					return false;
			}
			else
			{
				int64_t test = INT64_MAX;
				for (int i=0; i<10; ++i)
					if ((test -= i64) < 0)
						return false;
				if ((test -= val) < 0)
					return false;
			}

			i64 *= 10;
			i64 += val;
		}
	}
	out = (negative ? -i64 : i64);
	return true;
}*/

bool convert(const std::string& s, double& out) 
{ 
	int dot_cnt = 0;
	for (auto it=s.begin(); it!=s.end(); ++it)
	{
		if (*it == '-')
		{
			if (it != s.begin())
				return false;
		}
		else
		{
			if (isdigit(*it))
				continue;

			if (*it == '.' && dot_cnt++ == 0)
				continue;

			return false;
		}
	}
	out = atof(s.c_str()); 
	return true;
}

bool convert(const std::string& s, float& out) 
{ 
	double f;
	if (convert(s, f))
	{
		out = (float)f;
		return true;
	}
	return false;
}

/*bool convert(const std::string& s, std::string& out) 
{ 
	out = s;
	return true;
}*/
