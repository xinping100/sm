#include <iostream>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/sqlstring.h>
#include <cppconn/statement.h>
#include "dbsql.h"

int dbsql::execute(sql::Statement* stmt, sql::SQLString& str)
{
	try
	{
		stmt->execute(str);
		return execute_success;
	}
	catch (sql::SQLException& e)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

		return execute_fail;
	}
}

int dbsql::execute_query(sql::Statement* stmt, sql::SQLString& str, sql::ResultSet** result)
{
	try
	{
		sql::ResultSet* res = stmt->executeQuery(str);
		if (!res)
		{
			std::cout << "execute_query failed" << std::endl;
			return execute_fail;
		}
		*result = res;	
		return execute_success;
	}
	catch (sql::SQLException& e)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return execute_fail;
	}
}

int dbsql::execute_update(sql::Statement* stmt, sql::SQLString& str)
{
	try
	{
		int ret = stmt->executeUpdate(str);
		if (ret <= 0)
		{
			std::cout << "execute_update fail. ret = " << ret << std::endl;
			return execute_fail;
		}
		return execute_success;
	}
	catch (sql::SQLException& e)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return execute_fail;
	}
}

