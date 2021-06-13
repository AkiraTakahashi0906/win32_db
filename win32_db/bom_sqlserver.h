#ifndef INCLUDE_GUARD_BOM_SQLSERVER_HPP
#define INCLUDE_GUARD_BOM_SQLSERVER_HPP

#pragma once
#include "sqlserver_helper.h"
#include "bom_entity.h"
#include <tchar.h>
//#include "string_wstring_convert.h"

using namespace std;

class bom_sqlserver
{
private:
	static std::vector<bom_entity> _bom;
	static void getBoms(SQLHANDLE sqlStmtHandle);
public:
	bom_sqlserver();
	static std::vector<bom_entity> GetBoms();
	static void SaveBoms(std::vector<bom_entity> boms);
};

#endif

