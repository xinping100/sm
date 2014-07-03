#pragma once

#include "sharememoryserver.h"
#include "dbserverdata.h"

namespace sql
{
	class Statement;
	class Connection;
};

class dbserver
{
public:
	static dbserver* instance();
	static void release();
	~dbserver();
	void init();	
	void start();
	void term();
	sql::Statement* get_statement() const { return _statement; }
	sql::Connection* get_connection() const { return _connection; }
	unsigned long get_human_key() const { return _serverdata._humankey; }
	unsigned long get_human_count() const { return _serverdata._humancount; }
	unsigned long get_guild_key() const { return _serverdata._guildkey; }
	unsigned long get_guild_count() const { return _serverdata._guildcount; }

private:
	dbserver();
	void load_config();
	void open_connect();

	dbserver_data _serverdata;
	sql::Connection* _connection;
	sql::Statement* _statement;
	share_memory_server _smserver;
};


