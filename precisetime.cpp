#include <sys/time.h>
#include "precisetime.h"

unsigned int precise_time::_startmilisec = 0;
void precise_time::init()
{
	_startmilisec = current_milisec(); 
}

unsigned int precise_time::current_sec()
{
	return current_milisec();
}

unsigned int precise_time::current_milisec()
{
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	return tv.tv_sec*1000 + tv.tv_usec/1000; 
}

unsigned int precise_time::current_run_milisec() //miliseconds
{
	return current_milisec() - _startmilisec;
}

unsigned int precise_time::current_run_sec()
{
	return current_run_milisec() / 1000; 
}
