#pragma once
#include "vector"
#include "../task/task.h"
#include "../config/config.h"
#include "../database/db.h"
#include "../date/date.h"
#include <fstream>
#include <sstream>
#include <algorithm>

class ScheduleBuilder {
	ScheduleBuilder(); 
	static ScheduleBuilder *instance;
	DataBase *db;
	Config *config;
	std::vector<Task*> tasks;
public:
	static ScheduleBuilder* getInstance();
	~ScheduleBuilder() noexcept(false);
    ScheduleBuilder(ScheduleBuilder& other) = delete;
    void operator=(const ScheduleBuilder&) = delete;
	void addTask(Task* task);
	std::vector<Task*> getTasks();
	void fillTasks();
	void deleteTask(Task* task);
	void updateTaskCheck(Task* task);
	int totalPoints();
	int checkedPoints();
	Config* getConfig() { return config; };
	std::string* getNotes() { return config->getNotes(); };
	void uncheckAll();
};
