#ifndef INCLUDE_GUARD_BOM_ENTITY_HPP
#define INCLUDE_GUARD_BOM_ENTITY_HPP

#pragma once
#include <string>
using namespace std;

class bom_entity
{
public:
	bom_entity();
	bom_entity(int id, string name, time_t  date, bool is_deleted, double unit);
	~bom_entity();
	int get_id();
	string get_name();
	time_t get_date();
	bool get_is_deleted();
	double get_unit();

protected:
	int id;
	string name;
	time_t  date;
	bool is_deleted;
	double unit;
};

#endif

