#pragma once
#include <string>
#include "../time/time.h"

class __declspec(dllexport) Task {
	int id = 0;
	std::string name;
	unsigned int points;
	bool checked = false;
	Time *startTime;
	Time *endTime;
public:
	Task(std::string name, unsigned int points, Time *startTime, Time *endTime, bool checked = false);
	void setID(int id) { this->id = id; };
	void check(bool check) { this->checked = check; };
	bool* isChecked() { return &checked; };
	int getID() { return this->id; };
	std::string getName() { return name; };
	unsigned int getPoints() { return points; };
	Time* getStartTime() { return startTime; };
	Time* getEndTime() { return endTime; };
	std::string toString();
};
