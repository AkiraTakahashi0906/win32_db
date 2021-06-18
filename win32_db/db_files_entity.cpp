#include "db_files_entity.h"

db_files_entity::db_files_entity(
	int file_id,
	std::string type_desc,
	std::string name,
	int state,
	std::string state_desc,
	int size_MB,
	int space_used_MB,
	int max_size_MB) {

	this->file_id = file_id;
	this->type_desc = type_desc;
	this->name = name;
	this->state = state;
	this->state_desc = state_desc;
	this->size_MB = size_MB;
	this->space_used_MB = space_used_MB;
	this->max_size_MB = max_size_MB;
}

db_files_entity::~db_files_entity() {

}

int db_files_entity::get_size_MB(){
	return this->size_MB;
}