#include <iostream>
#include <fstream>
#include "sharememoryobject.h"
#include "tablehumanoper.h"

void share_memmory_logic(struct smo_player* o)
{
	bool dirty = false;
	table_human tabledata;
	o->lock();
	dirty = o->_dirty;
	o->set_dirty(false);
	if (dirty)
		tabledata = o->_tabledata;
	o->unlock(); 
	
	if (dirty)
	{
		table_human_oper oper;
		oper.save(tabledata);	
	}
}
