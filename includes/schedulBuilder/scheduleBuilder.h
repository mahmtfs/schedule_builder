#pragma once
#include "vector"
#include "../task/task.h"
#include "../database/db.h"
#include "rapidxml.hpp"
#include "rapidxml_iterators.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

class ScheduleBuilder {
	ScheduleBuilder(std::string path); 
	static ScheduleBuilder* instance;
	DataBase *db;
	std::vector<Task*> tasks;
public:
	static ScheduleBuilder* getInstance(std::string path);
	~ScheduleBuilder() { db->close(); };
    ScheduleBuilder(ScheduleBuilder& other) = delete;
    void operator=(const ScheduleBuilder&) = delete;
	void addTask(Task* task);
	std::vector<Task*> getTasks();
	void fillTasks();
	void deleteTask(Task* task);
	void updateTaskCheck(Task* task);
	int totalPoints();
	int checkedPoints();
};
