#include "bom_entity.h"

bom_entity::bom_entity() {
	this->id = 0;
	this->name = "";
}

bom_entity::bom_entity(int id, string name, time_t  date, bool is_deleted, double unit) {
	this->id = id;
	this->name = name;
	this->date = date;
	this->is_deleted = is_deleted;
	this->unit = unit;
}

bom_entity::~bom_entity() {

}

int bom_entity::get_id() {
	return this->id;
}

string bom_entity::get_name() {
	return this->name;
}

time_t bom_entity::get_date() {
	return this->date;
}

bool bom_entity::get_is_deleted() {
	return this->is_deleted;
}

double bom_entity::get_unit() {
	return this->unit;
}