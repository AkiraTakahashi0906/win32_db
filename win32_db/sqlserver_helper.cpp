#include "sqlserver_helper.h"
using namespace std;

SQLHANDLE sqlserver_helper::m_sqlConnHandle ;
SQLHANDLE sqlserver_helper::m_sqlStmtHandle;
SQLHANDLE sqlserver_helper::m_sqlEnvHandle;
SQLWCHAR sqlserver_helper::m_retconstring[SQL_RETURN_CODE_LEN];

sqlserver_helper::sqlserver_helper() {
}

sqlserver_helper::~sqlserver_helper() {
    connection_close();
}

void sqlserver_helper::get_error_message() {
    SQLSMALLINT nSize;
    TCHAR       szState[6];
    TCHAR       szErrorMsg[1024];
    SQLINTEGER  nErrorCode;

    SQLGetDiagRec(SQL_HANDLE_STMT, m_sqlStmtHandle, 1, (SQLTCHAR*)szState, &nErrorCode, (SQLTCHAR*)szErrorMsg, sizeof(szErrorMsg) / sizeof(TCHAR), &nSize);
    MessageBox(NULL, szErrorMsg, NULL, MB_ICONWARNING);

}

void sqlserver_helper::BeginTran() {
    sqlserver_helper::connection_open();
    SQLSetConnectOption(m_sqlConnHandle, SQL_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF);
}

void sqlserver_helper::RollBack() {
    SQLTransact(m_sqlEnvHandle, m_sqlConnHandle, SQL_ROLLBACK);
    sqlserver_helper::connection_close();
}

void sqlserver_helper::EndTran() {
    SQLTransact(m_sqlEnvHandle, m_sqlConnHandle, SQL_COMMIT);
    sqlserver_helper::connection_close();
}

void sqlserver_helper::connection_close() {
    SQLFreeHandle(SQL_HANDLE_STMT, m_sqlStmtHandle);
    SQLDisconnect(m_sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, m_sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, m_sqlEnvHandle);
}

bool sqlserver_helper::connection_open() {

    //initializations
    m_sqlConnHandle = NULL;
    m_sqlStmtHandle = NULL;
    m_sqlEnvHandle;
    m_retconstring[SQL_RETURN_CODE_LEN];

    //allocations
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_sqlEnvHandle))
        goto COMPLETED;

    if (SQL_SUCCESS != SQLSetEnvAttr(m_sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
        goto COMPLETED;

    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, m_sqlEnvHandle, &m_sqlConnHandle))
        goto COMPLETED;

    switch (SQLDriverConnect(m_sqlConnHandle,
        NULL,
        (SQLWCHAR*)conn_str.c_str(),
        SQL_NTS,
        m_retconstring,
        1024,
        NULL,
        SQL_DRIVER_NOPROMPT))
    {
    case SQL_SUCCESS:
        //std::cout << "Successfuly\n";
        break;

    case SQL_SUCCESS_WITH_INFO:
        //std::cout << "Successfuly\n";
        break;

    case SQL_INVALID_HANDLE:
        std::cout << "Could not connect\n";
        goto COMPLETED;

    case SQL_ERROR:
        std::cout << "Could not connect\n";
        goto COMPLETED;

    default:
        std::cout << "Could not connect\n";
        goto COMPLETED;
    }

    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, m_sqlConnHandle, &m_sqlStmtHandle))
        goto COMPLETED;

    return true;

COMPLETED:
    sqlserver_helper::connection_close();
    return false;
}

void sqlserver_helper::Query(wstring sql, Func func) {

    if (sqlserver_helper::connection_open()) {
        if (SQL_SUCCESS != SQLExecDirect(m_sqlStmtHandle, (SQLWCHAR*)sql.c_str(), SQL_NTS)) {
            std::cout << "Error querying SQL Server\n";
            sqlserver_helper::get_error_message();
            goto COMPLETED;
        }
        else {
            while (SQLFetch(m_sqlStmtHandle) == SQL_SUCCESS) {
                func(m_sqlStmtHandle);
            }
        }
    }
COMPLETED:
    sqlserver_helper::connection_close();
}

void sqlserver_helper::Query(wstring sql, Func func, std::vector<sql_parameter> parameters) {
    SQLRETURN   nResult;
    if (sqlserver_helper::connection_open()) {

        SQLSetConnectOption(m_sqlConnHandle, SQL_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF);

        nResult = SQLPrepare(m_sqlStmtHandle, (SQLTCHAR*)sql.c_str(), SQL_NTS);
        if (nResult != SQL_SUCCESS && nResult != SQL_SUCCESS_WITH_INFO)
            goto NOT_COMPLETED;

        for (auto itr = parameters.begin(); itr != parameters.end(); ++itr) {
            SQLBindParameter(m_sqlStmtHandle,
                itr._Ptr->ParameterNumber,
                itr._Ptr->InputOutputType,
                itr._Ptr->ValueType,
                itr._Ptr->ParameterType,
                itr._Ptr->ColumnSize,
                itr._Ptr->DecimalDigits,
                itr._Ptr->ParameterValuePtr,
                itr._Ptr->BufferLength,
                itr._Ptr->StrLen_or_IndPtr);
        }

        nResult = SQLExecute(m_sqlStmtHandle);
        if (nResult != SQL_SUCCESS && nResult != SQL_SUCCESS_WITH_INFO) {
            std::cout << "Error querying SQL Server\n";
            sqlserver_helper::get_error_message();
            goto NOT_COMPLETED;
        }
        else {
            while (SQLFetch(m_sqlStmtHandle) == SQL_SUCCESS) {
                func(m_sqlStmtHandle);
            }
        }
        SQLTransact(m_sqlEnvHandle, m_sqlConnHandle, SQL_COMMIT);
        sqlserver_helper::connection_close();
        return;
    }
NOT_COMPLETED:
    SQLTransact(m_sqlEnvHandle, m_sqlConnHandle, SQL_ROLLBACK);
    sqlserver_helper::connection_close();
}

void sqlserver_helper::Execute(wstring sql, std::vector<sql_parameter> parameters) {
    SQLLEN    nRowCount = 0;
    SQLRETURN   nResult;

    if (sqlserver_helper::connection_open()) {
        SQLSetConnectOption(m_sqlConnHandle, SQL_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF);
        nResult = SQLPrepare(m_sqlStmtHandle, (SQLTCHAR*)sql.c_str(), SQL_NTS);
        if (nResult != SQL_SUCCESS && nResult != SQL_SUCCESS_WITH_INFO)
            goto NOT_COMPLETED;

        for (auto itr = parameters.begin(); itr != parameters.end(); ++itr) {
            SQLBindParameter(m_sqlStmtHandle,
                itr._Ptr->ParameterNumber,
                itr._Ptr->InputOutputType,
                itr._Ptr->ValueType,
                itr._Ptr->ParameterType,
                itr._Ptr->ColumnSize,
                itr._Ptr->DecimalDigits,
                itr._Ptr->ParameterValuePtr,
                itr._Ptr->BufferLength,
                itr._Ptr->StrLen_or_IndPtr);
        }

        nResult = SQLExecute(m_sqlStmtHandle);
        if (nResult != SQL_SUCCESS && nResult != SQL_SUCCESS_WITH_INFO) {
            std::cout << "Error querying SQL Server\n";
            sqlserver_helper::get_error_message();
            goto NOT_COMPLETED;
        }
        else {
            SQLRowCount(m_sqlStmtHandle, &nRowCount);
            SQLTransact(m_sqlEnvHandle, m_sqlConnHandle, SQL_COMMIT);
            sqlserver_helper::connection_close();
            return;
        }
    }

NOT_COMPLETED:
    SQLTransact(m_sqlEnvHandle, m_sqlConnHandle, SQL_ROLLBACK);
    sqlserver_helper::connection_close();
}

void sqlserver_helper::Execute(wstring sql) {
    SQLLEN    nRowCount = 0;
    SQLRETURN nResult;

    nResult = SQLExecDirect(m_sqlStmtHandle, (SQLWCHAR*)sql.c_str(), SQL_NTS);
    if (nResult != SQL_SUCCESS && nResult != SQL_SUCCESS_WITH_INFO) {// && nResult != SQL_NO_DATA) {
        std::cout << "Error querying SQL Server\n";
        //sqlserver_helper::get_error_message();
        throw std::exception();
    }
    SQLRowCount(m_sqlStmtHandle, &nRowCount);
}

sql_parameter sqlserver_helper::get_integer_parameter(int parameterNumber, SQLSMALLINT* nParameter_ptr, SQLINTEGER* nParameterSize_ptr) {

    struct sql_parameter parameter;
    parameter.ParameterNumber = parameterNumber;
    parameter.InputOutputType = SQL_PARAM_INPUT;
    parameter.ValueType = SQL_C_SSHORT;
    parameter.ParameterType = SQL_INTEGER;
    parameter.ColumnSize = 0;
    parameter.DecimalDigits = 0;
    parameter.ParameterValuePtr = nParameter_ptr;
    parameter.BufferLength = 0;
    parameter.StrLen_or_IndPtr = nParameterSize_ptr;

    return parameter;
}

sql_parameter sqlserver_helper::get_string_parameter(int parameterNumber, SQLPOINTER nParameter_ptr, SQLINTEGER* nParameterSize_ptr) {

    struct sql_parameter parameter;
    parameter.ParameterNumber = parameterNumber;
    parameter.InputOutputType = SQL_PARAM_INPUT;
    parameter.ValueType = SQL_C_WCHAR;
    parameter.ParameterType = SQL_WVARCHAR;
    parameter.ColumnSize = 20;
    parameter.DecimalDigits = 0;
    parameter.ParameterValuePtr = nParameter_ptr;
    parameter.BufferLength = 0;
    parameter.StrLen_or_IndPtr = nParameterSize_ptr;

    return parameter;
}

sql_parameter sqlserver_helper::get_time_parameter(int parameterNumber, SQLPOINTER nParameter_ptr, SQLINTEGER* nParameterSize_ptr) {

    struct sql_parameter parameter;
    parameter.ParameterNumber = parameterNumber;
    parameter.InputOutputType = SQL_PARAM_INPUT;
    parameter.ValueType = SQL_C_TIMESTAMP;
    parameter.ParameterType = SQL_TIMESTAMP;
    parameter.ColumnSize = 21;
    parameter.DecimalDigits = 0;
    parameter.ParameterValuePtr = nParameter_ptr;
    parameter.BufferLength = 0;
    parameter.StrLen_or_IndPtr = nParameterSize_ptr;

    return parameter;
}

sql_parameter sqlserver_helper::get_bool_parameter(int parameterNumber, SQLPOINTER nParameter_ptr, SQLINTEGER* nParameterSize_ptr) {

    struct sql_parameter parameter;
    parameter.ParameterNumber = parameterNumber;
    parameter.InputOutputType = SQL_PARAM_INPUT;
    parameter.ValueType = SQL_C_BIT;
    parameter.ParameterType = SQL_BIT;
    parameter.ColumnSize = 1;
    parameter.DecimalDigits = 0;
    parameter.ParameterValuePtr = nParameter_ptr;
    parameter.BufferLength = 0;
    parameter.StrLen_or_IndPtr = nParameterSize_ptr;

    return parameter;
}

sql_parameter sqlserver_helper::get_float_parameter(int parameterNumber, SQLPOINTER nParameter_ptr, SQLINTEGER* nParameterSize_ptr) {

    struct sql_parameter parameter;
    parameter.ParameterNumber = parameterNumber;
    parameter.InputOutputType = SQL_PARAM_INPUT;
    parameter.ValueType = SQL_C_DOUBLE;
    parameter.ParameterType = SQL_DOUBLE;
    parameter.ColumnSize = 0;
    parameter.DecimalDigits = 0;
    parameter.ParameterValuePtr = nParameter_ptr;
    parameter.BufferLength = 0;
    parameter.StrLen_or_IndPtr = nParameterSize_ptr;

    return parameter;
}

sql_parameter sqlserver_helper::get_insert_string_parameter(int parameterNumber, SQLPOINTER nParameter_ptr, SQLINTEGER* nParameterSize_ptr) {

    struct sql_parameter parameter;
    parameter.ParameterNumber = parameterNumber;
    parameter.InputOutputType = SQL_PARAM_INPUT;
    parameter.ValueType = SQL_C_WCHAR;
    parameter.ParameterType = SQL_WVARCHAR;
    parameter.ColumnSize = 50;
    parameter.DecimalDigits = 0;
    parameter.ParameterValuePtr = nParameter_ptr;
    parameter.BufferLength = 0;
    parameter.StrLen_or_IndPtr = nParameterSize_ptr;

    return parameter;
}
