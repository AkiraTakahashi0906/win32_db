#include "bom_sqlserver.h"
std::vector<bom_entity> bom_sqlserver::_bom;

bom_sqlserver::bom_sqlserver() {

}

void bom_sqlserver::getBoms(SQLHANDLE sqlStmtHandle) {

	SQLINTEGER result;
	SQLINTEGER ptrResult;

	//SQLCHAR result2[SQL_RESULT_LEN];
	SQLCHAR result2[256];
	SQLINTEGER ptrResult2;

	TIMESTAMP_STRUCT result3;
	SQLINTEGER ptrResult3;

	bool result4;
	SQLINTEGER ptrResult4;

	double result5;
	//SQLINTEGER ptrResult5;

	SQLGetData(sqlStmtHandle, 1, SQL_INTEGER, &result, SQL_RESULT_LEN, &ptrResult);
	SQLGetData(sqlStmtHandle, 3, SQL_TIMESTAMP, &result3, SQL_RESULT_LEN, &ptrResult3);
	SQLGetData(sqlStmtHandle, 4, SQL_BIT, &result4, SQL_RESULT_LEN, &ptrResult4);
	SQLGetData(sqlStmtHandle, 5, SQL_DOUBLE, &result5, SQL_RESULT_LEN, NULL);
	SQLGetData(sqlStmtHandle, 6, SQL_CHAR, &result2, SQL_RESULT_LEN, &ptrResult2);

	auto* str = result2;
	string sss = string((char*)result2);

	time_t tim;
	struct tm stm1;
	stm1.tm_year = result3.year - 1900;
	stm1.tm_mon = result3.month - 1;
	stm1.tm_mday = result3.day;
	stm1.tm_hour = result3.hour;
	stm1.tm_min = result3.minute;
	stm1.tm_sec = result3.second;
	tim = mktime(&stm1);
	bom_sqlserver::_bom.push_back(bom_entity(result, sss, tim, result4, result5));

}

std::vector<bom_entity> bom_sqlserver::GetBoms() {
	sqlserver_helper helper;
	wstring sql = L"\
SELECT\
	[id] ,[assy_number] ,[date2] ,[is_deleted],[unit] ,[parts_name]\
 from bom \
	where [id] = ? and [assy_number] = ?  and [date2] < ? and [is_deleted] = ?  and [unit] = ? ";

	std::vector<sql_parameter> parameters;
	SQLSMALLINT nParameter = 15;
	//SQLINTEGER  nParameterSize=0;
	struct  sql_parameter parameter1 = helper.get_integer_parameter(1, &nParameter, NULL);
	parameters.push_back(parameter1);

	SQLWCHAR Name[256] = { '0' };
	SQLLEN nRes_Name = SQL_NTS;
	wcscpy_s(Name, L"1AAAA");
	struct  sql_parameter parameter2 = helper.get_string_parameter(2, &Name, &nRes_Name);
	parameters.push_back(parameter2);

	TIMESTAMP_STRUCT ts;
	ts.year = 2022;
	ts.month = 12;
	ts.day = 7;
	ts.hour = 0;
	ts.minute = 0;
	ts.second = 0;
	ts.fraction = 0;

	struct  sql_parameter parameter3 = helper.get_time_parameter(3, &ts, NULL);
	parameters.push_back(parameter3);

	bool is_deleted = true;
	struct  sql_parameter parameter4 = helper.get_bool_parameter(4, &is_deleted, NULL);
	parameters.push_back(parameter4);

	double unit = 0.123;
	struct  sql_parameter parameter5 = helper.get_float_parameter(5, &unit, NULL);
	parameters.push_back(parameter5);

	helper.Query(sql, &bom_sqlserver::getBoms, parameters);
	return _bom;
}

void bom_sqlserver::SaveBoms(std::vector<bom_entity> boms) {

	sqlserver_helper helper;
	std::vector<sql_parameter> parameters;
	wstring sql = L"";

	for (auto itr = boms.begin(); itr != boms.end(); ++itr) {
		sql = L"\
insert into bom \
	(assy_number,unit,parts_number,is_deleted,date2)\
values\
	 (?,?,?,?,?) ";
		SQLSMALLINT nParameter = itr._Ptr->get_id() + 100;
		struct  sql_parameter parameter1 = helper.get_integer_parameter(1, &nParameter, NULL);
		parameters.push_back(parameter1);

		double unit = itr._Ptr->get_unit() + 100;
		struct  sql_parameter parameter2 = helper.get_float_parameter(2, &unit, NULL);
		parameters.push_back(parameter2);

		SQLWCHAR Name[1024] = { '0' };
		SQLLEN nRes_Name = SQL_NTS;

		std::wstring ws2 = string_wstring_convert::StringToWString(itr._Ptr->get_name());
		wchar_t* wc2 = ws2.data();

		wcscpy_s(Name, wc2);
		struct  sql_parameter parameter3 = helper.get_string_parameter(3, &Name, &nRes_Name);
		parameters.push_back(parameter3);

		bool is_deleted = itr._Ptr->get_is_deleted();
		struct  sql_parameter parameter4 = helper.get_bool_parameter(4, &is_deleted, NULL);
		parameters.push_back(parameter4);

		errno_t error;
		time_t jikan = itr._Ptr->get_date();
		struct tm imanojikan;
		error = localtime_s(&imanojikan, &jikan);
		TIMESTAMP_STRUCT ts;
		ts.year = imanojikan.tm_year + 1900;
		ts.month = imanojikan.tm_mon + 1;
		ts.day = imanojikan.tm_mday;
		ts.hour = imanojikan.tm_hour;
		ts.minute = imanojikan.tm_min;
		ts.second = imanojikan.tm_sec;
		ts.fraction = 0;

		error = localtime_s(&imanojikan, &jikan);

		struct  sql_parameter parameter5 = helper.get_time_parameter(5, &ts, NULL);
		parameters.push_back(parameter5);

		helper.Execute(sql, parameters);
	}
}
