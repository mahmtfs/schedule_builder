#include "scheduleBuilder.h"

ScheduleBuilder* ScheduleBuilder::instance = nullptr;

ScheduleBuilder::ScheduleBuilder(std::string path) {
	db = DataBase::getInstance(path);
	fillTasks();
}

ScheduleBuilder* ScheduleBuilder::getInstance(std::string path) {
	if (instance == nullptr) {
		instance = new ScheduleBuilder(path);
	}
	return instance;
}

std::vector<Task*> ScheduleBuilder::getTasks() {
	return tasks;
}

void ScheduleBuilder::addTask(Task* task) {
	tasks.push_back(task);
	std::sort(tasks.begin(), tasks.end(), [](Task* a, Task* b) {return a->getTime()->getTime() < b->getTime()->getTime(); });
	db->addTask(task);
}

void ScheduleBuilder::fillTasks() {
	tasks.clear();
	for (Task* task : db->getTasks()) {
		this->tasks.push_back(task);
	}
}

void ScheduleBuilder::deleteTask(Task* task) {
	db->deleteTask(task);
	for (int i = 0; i < this->tasks.size(); i+=1) {
		if (this->tasks[i]->getID() == task->getID()) {
			this->tasks.erase(this->tasks.begin() + i);
		}
	}
}

void ScheduleBuilder::updateTaskCheck(Task* task) {
	db->updateTaskCheck(task);
}
