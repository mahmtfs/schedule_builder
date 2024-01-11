#pragma once
#include <string>

class Task {
	std::string name;
	unsigned int points;
	bool checked = false;
public:
	Task(std::string name, unsigned int points, bool checked=false);
	bool* isChecked() { return &checked; };
	std::string getName() { return name; };
	unsigned int getPoints() { return points; };
	std::string toString();
};
