#pragma once
#include <string>
#include "../time/time.h"

class Task {
	int id = 0;
	std::string name;
	unsigned int points;
	bool checked = false;
	Time *time;
public:
	Task(std::string name, unsigned int points, Time *time, bool checked = false);
	void setID(int id) { this->id = id; };
	bool* isChecked() { return &checked; };
	int getID() { return this->id; };
	std::string getName() { return name; };
	unsigned int getPoints() { return points; };
	Time* getTime() { return time; };
	std::string toString();
};
