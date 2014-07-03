#pragma once

namespace sql
{
	class Statement;
	class SQLString;
}

class dbsql
{
public:
	enum 
	{
		execute_fail,
		execute_success,
	};
	//create/drop table/use database
	static int execute(sql::Statement*, sql::SQLString& str); 
	//for "select" operation
	static int execute_query(sql::Statement*, sql::SQLString& str, sql::ResultSet** result); 
	//for "insert/update/delete"
	static int execute_update(sql::Statement*, sql::SQLString& str); 
};
