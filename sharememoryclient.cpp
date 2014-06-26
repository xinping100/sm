#include "sharememoryclient.h"
#include "sharememorykey.h"
#include "sharememoryenvironment.h"

bool share_memory_client::init()
{
	share_memory_init_environment(share_memory_environment_client);
	_msgqueue.init();
	_playermanager.init(SM_KEY_PLAYER, 1024);
	return true;
}

bool share_memory_client::term()
{
	_playermanager.term();
	_msgqueue.term();
	return true;
}

