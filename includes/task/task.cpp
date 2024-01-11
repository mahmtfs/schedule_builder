#include "task.h"

Task::Task(std::string name, unsigned int points, bool checked) {
	this->name = name;
	this->points = points;
	this->checked = checked;
}

std::string Task::toString() {
	return this->name + " | " + std::to_string(this->points) + " | " + (this->checked ? "checked" : "unchecked");
}
