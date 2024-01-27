#pragma once
#include <string>

class __declspec(dllexport) Time {
	std::string hours;
	std::string minutes;
	std::string seconds;
public:
	Time(std::string time); //00:00:00 time format is expected
	Time(std::string hours, std::string minutes, std::string seconds);
	std::string getHours() { return hours; };
	std::string getminutes() { return minutes; };
	std::string getSeconds() { return seconds; };
	std::string getTime() { return hours + ":" + minutes + ":" + seconds; };
	static bool correctTime(int hours, int minutes, int seconds);
	static bool correctSequence(int startHours, int startMinutes, int startSeconds, int endHours, int endMinutes, int endSeconds);
};
