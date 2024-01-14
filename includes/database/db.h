#pragma once
#include "../../libs/sqlite/sqlite3.h"
#include "../task/task.h"
#include <string>
#include <vector>
#include <iostream>

class DataBase {
	DataBase(std::string path);
	static DataBase* instance;
	sqlite3* db;
public:
	static DataBase* getInstance(std::string path);
	DataBase(DataBase& other) = delete;
	void operator=(const DataBase&) = delete;
	std::vector<Task*> getTasks();
	void addTask(Task* task);
	void deleteTask(Task* task);
	void updateTaskCheck(Task* task);
	void uncheckAll();
	void close() { sqlite3_close(db); };
};
