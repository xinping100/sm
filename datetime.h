#pragma once

#include <time.h>
class date_time
{
public:
	date_time();
	int year() const;
	int month() const;
	int yday() const; //day in a year (0~365)
	int mday() const;//day in a month (0~31)
	int wday() const;//day in a week (0~6)
	int hour() const;
	int minute() const;
	int second() const;

private:
	struct tm _localtime;
};
