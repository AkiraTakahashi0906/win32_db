#ifndef INCLUDE_GUARD_SQLSERVER_HELPER_HPP
#define INCLUDE_GUARD_SQLSERVER_HELPER_HPP

#pragma once
#include <Windows.h>
#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <vector>
#include "string_wstring_convert.h"

using namespace std;

#define SQL_RESULT_LEN 1024
#define SQL_RETURN_CODE_LEN 2048

using Func = void (*)(SQLHANDLE x);
const wstring conn_str = L"DRIVER={SQL Server};SERVER=DESKTOP-8B0KCU1\\SQLEXPRESS;DATABASE=KGWS;UID=python;PWD=akira;";

struct sql_parameter {
	SQLUSMALLINT     ParameterNumber;    /* In */
	SQLSMALLINT       InputOutputType;     /* In */
	SQLSMALLINT       ValueType;               /* In */
	SQLSMALLINT       ParameterType;        /* In */
	SQLULEN              ColumnSize;             /* In */
	SQLSMALLINT       DecimalDigits;          /* In */
	SQLPOINTER         ParameterValuePtr;  /* In */
	SQLLEN                BufferLength;          /* In */
	SQLLEN* StrLen_or_IndPtr;    /* In */
};

class sqlserver_helper
{
public:
	sqlserver_helper(void);
	~sqlserver_helper(void);
	void Query(wstring sql, Func func);
	void Query(wstring sql, Func func, std::vector<sql_parameter> parameters);
	void Execute(wstring sql);
	void Execute(wstring sql, std::vector<sql_parameter> parameters);
	void BeginTran();
	void RollBack();
	void EndTran();
	sql_parameter get_integer_parameter(int parameterNumber, SQLSMALLINT* nParameter_ptr, SQLINTEGER* nParameterSize_ptr);
    sql_parameter get_string_parameter(int parameterNumber, SQLPOINTER nParameter_ptr, SQLINTEGER* nParameterSize_ptr);
    sql_parameter get_time_parameter(int parameterNumber, SQLPOINTER nParameter_ptr, SQLINTEGER* nParameterSize_ptr);
    sql_parameter get_bool_parameter(int parameterNumber, SQLPOINTER nParameter_ptr, SQLINTEGER* nParameterSize_ptr);
    sql_parameter get_float_parameter(int parameterNumber, SQLPOINTER nParameter_ptr, SQLINTEGER* nParameterSize_ptr);
    sql_parameter get_insert_string_parameter(int parameterNumber, SQLPOINTER nParameter_ptr, SQLINTEGER* nParameterSize_ptr);

private:
	static SQLHANDLE m_sqlConnHandle;
	static SQLHANDLE m_sqlStmtHandle;
	static SQLHANDLE m_sqlEnvHandle;
	static SQLWCHAR m_retconstring[SQL_RETURN_CODE_LEN];
	bool connection_open();
	void connection_close();
	void get_error_message();
};

#endif