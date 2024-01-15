#include "scheduleBuilder.h"

ScheduleBuilder* ScheduleBuilder::instance = nullptr;

ScheduleBuilder::ScheduleBuilder() {
	config = new Config("C:/Portfolio/projects/schedule_builder/config/config.xml");
	db = DataBase::getInstance(config->getDatabasePath());
	std::string currentDate = Date::getCurrentDate();
	if (currentDate > config->getLastVisited()) {
		uncheckAll();
		config->setLastVisited(currentDate);
	}
	fillTasks();
}

ScheduleBuilder::~ScheduleBuilder() noexcept(false) {
	config->setNotes(*getNotes());
}

ScheduleBuilder* ScheduleBuilder::getInstance() {
	if (instance == nullptr) {
		instance = new ScheduleBuilder();
	}
	return instance;
}

std::vector<Task*> ScheduleBuilder::getTasks() {
	return tasks;
}

void ScheduleBuilder::addTask(Task* task) {
	tasks.push_back(task);
	std::sort(tasks.begin(), tasks.end(), [](Task* a, Task* b) {return a->getEndTime()->getTime() < b->getEndTime()->getTime(); });
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
	db->uncheckAll();
	for (Task* task : this->tasks) {
		task->check(false);
	}
}
