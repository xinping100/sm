#pragma once

class precise_time
{
public:
	static void init();
	static unsigned int current_sec(); //since 1970
	static unsigned int current_milisec(); //since 1970
	static unsigned int current_run_milisec(); //miliseconds
	static unsigned int current_run_sec();

private:
	static unsigned int _startmilisec;
};
