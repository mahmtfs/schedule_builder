#include "task.h"

Task::Task(std::string name, unsigned int points, Time* startTime, Time* endTime, bool checked) {
	this->name = name;
	this->points = points;
	this->checked = checked;
	this->startTime = startTime;
	this->endTime = endTime;
}

std::string Task::toString() {
	return this->name + " | " + std::to_string(this->points) + " | " + this->getStartTime()->getTime() + " - " + this->getEndTime()->getTime();
}
