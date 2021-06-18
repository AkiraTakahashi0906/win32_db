#ifndef INCLUDE_GUARD_DB_FILES_ENTITY_HPP
#define INCLUDE_GUARD_DB_FILES_ENTITY_HPP

#pragma once
#include <string>
class db_files_entity
{
public:
	db_files_entity(int file_id,
						  std::string type_desc,
						  std::string name,
						  int state,
						  std::string state_desc,
						  int size_MB,
						  int space_used_MB,
		                  int max_size_MB);
	~db_files_entity();

	int get_size_MB();
	
protected:
	int file_id;
	std::string type_desc;
	std::string name;
	int state;
	std::string state_desc;
	int size_MB;
	int space_used_MB;
	int max_size_MB;
};

#endif

