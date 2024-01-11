#pragma once
#include "vector"
#include "../task/task.h"
#include "rapidxml.hpp"
#include "rapidxml_iterators.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
#include <fstream>
#include <sstream>

class ScheduleBuilder {
	ScheduleBuilder(std::string configPath); 
	static ScheduleBuilder* instance;
	std::string configPath;
	std::vector<Task*> tasks;
public:
	static ScheduleBuilder* getInstance(std::string configPath);
    ScheduleBuilder(ScheduleBuilder& other) = delete;
    void operator=(const ScheduleBuilder&) = delete;
	void addTask(Task* task);
	std::vector<Task*> getTasks();
	void fillTasks(std::string filePath);
	void check(Task* task);
};
