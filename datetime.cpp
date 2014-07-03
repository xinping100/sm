#include <time.h>
#include "datetime.h"

date_time::date_time()
{
	time_t _time = time(nullptr);
	localtime_r(&_time, &_localtime);
}

int date_time::year() const
{
	return _localtime.tm_year;
}

int date_time::month() const
{
	return _localtime.tm_mon;
}

int date_time::yday() const //day in a year (0~365)
{
	return _localtime.tm_yday;
}

int date_time::mday() const//day in a month (0~31)
{
	return _localtime.tm_mday;
}

int date_time::wday() const//day in a week (0~6)
{
	return _localtime.tm_wday;
}

int date_time::hour() const
{
	return _localtime.tm_hour;
}

int date_time::minute() const
{
	return _localtime.tm_min;
}

int date_time::second() const
{
	return _localtime.tm_sec;
}

