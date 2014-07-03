#include <iostream>
#include <assert.h>
#include <unistd.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include "dbserver.h"
#include "config.h"
#include "sharememorykey.h"
#include "sharememorylogic.h"
#include "sharememoryenvironment.h"

dbserver* dbserver::instance()
{
	static dbserver* tmp = new dbserver;
	return tmp;
}

void dbserver::release()
{
	delete instance();
}

dbserver::~dbserver()
{
}

void dbserver::init()
{
	load_config();
	open_connect();
	_smserver.init();
}

void dbserver::start()
{
	_smserver.loop();
}

void dbserver::term()
{
	if (_connection)
	{
		_connection->close();
		delete _connection;
	}

	if (_statement)
	{
		_statement->close();
		delete _statement;
	}

	_smserver.term();
}
dbserver::dbserver()
	: _connection(nullptr), _statement(nullptr)
{
}

void dbserver::load_config()
{
	config c("dbconfig");
	_serverdata._dbip = c.read_string("dbip");
	_serverdata._dbname = c.read_string("dbname");
	_serverdata._username = c.read_string("username");	
	_serverdata._userpasswd = c.read_string("userpasswd");
	_serverdata._humankey = c.read_long("humankey");
	_serverdata._humancount = c.read_int("humancount");
	_serverdata._humansaveinterval = c.read_int("humansaveinterval");
	_serverdata._guildkey = c.read_long("guildkey");
	_serverdata._guildcount = c.read_int("guildcount");
	_serverdata._guildsaveinterval = c.read_int("guildsaveinterval");
}

void dbserver::open_connect()
{
	sql::Driver* driver = get_driver_instance();
	assert(driver);
	try
	{
		_connection = driver->connect(_serverdata._dbip, _serverdata._username, _serverdata._userpasswd);
		assert(_connection);
		_connection->setSchema(_serverdata._dbname);
		_statement = _connection->createStatement();
		assert(_statement);
	}
	catch (sql::SQLException& e)
	{
		std::cout << "driver->connect exception occurs: " << e.what() << std::endl;
	}
}


