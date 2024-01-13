#include "task.h"

Task::Task(std::string name, unsigned int points, Time *time, bool checked) {
	this->name = name;
	this->points = points;
	this->checked = checked;
	this->time = time;
}

std::string Task::toString() {
	return this->name + " | " + std::to_string(this->points) + " | " + this->getTime()->getTime();
}
