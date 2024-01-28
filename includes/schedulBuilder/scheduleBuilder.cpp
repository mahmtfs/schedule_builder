#include "scheduleBuilder.h"

ScheduleBuilder* ScheduleBuilder::instance = nullptr;

ScheduleBuilder::ScheduleBuilder() {
	this->config = new Config(".\\.\\config\\config.xml");
	this->db = DataBase::getInstance(this->config->getDatabasePath());
	std::string currentDate = Date::getCurrentDate();
	if (currentDate > this->config->getLastVisited()) {
		uncheckAll();
		this->config->setLastVisited(currentDate);
	}
	fillTasks();
}

ScheduleBuilder* ScheduleBuilder::getInstance() {
	if (instance == nullptr) {
		instance = new ScheduleBuilder();
	}
	return instance;
}

std::vector<Task*> ScheduleBuilder::getTasks() {
	return this->tasks;
}

void ScheduleBuilder::addTask(Task* task) {
	this->tasks.push_back(task);
	std::sort(this->tasks.begin(), this->tasks.end(), [](Task* a, Task* b) {return a->getEndTime()->getTime() < b->getEndTime()->getTime(); });
	this->db->addTask(task);
}

void ScheduleBuilder::fillTasks() {
	this->tasks.clear();
	for (Task* task : db->getTasks()) {
		this->tasks.push_back(task);
	}
}

void ScheduleBuilder::deleteTask(Task* task) {
	this->db->deleteTask(task);
	for (int i = 0; i < this->tasks.size(); i+=1) {
		if (this->tasks[i]->getID() == task->getID()) {
			this->tasks.erase(this->tasks.begin() + i);
		}
	}
}

void ScheduleBuilder::updateTaskCheck(Task* task) {
	this->db->updateTaskCheck(task);
}

int ScheduleBuilder::totalPoints() {
	int sum = 0;
	for (Task* task : this->tasks) {
		sum += task->getPoints();
	}
	return sum;
}

int ScheduleBuilder::checkedPoints() {
	int sum = 0;
	for (Task* task : this->tasks) {
		if (*(task->isChecked())) {
			sum += task->getPoints();
		}
	}
	return sum;
}

void ScheduleBuilder::uncheckAll() {
	this->db->uncheckAll();
	for (Task* task : this->tasks) {
		task->check(false);
	}
}
