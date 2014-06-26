#include <iostream>
#include <fstream>
#include "sharememoryobject.h"

void share_memmory_logic(struct smo_player* o)
{
	o->lock();

	std::ofstream f;
	char id[16] = {};
	sprintf(id, "%d", o->id);
	f.open(id, std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
	f.write((char*)&o->id, sizeof(o->id));
	f.write((char*)&o->level, sizeof(o->level));
	f.write((char*)&o->money, sizeof(o->money));
	//f << o->id << o->level << o->money;
	f.flush();
	f.close();

	o->unlock();
	std::cout << "save player to file" 
		<< ",id=" << o->id 
		<< ",level=" << o->level
	   	<< ",money" << o->money 
		<< std::endl;
}
