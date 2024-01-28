#include "time.h"

Time::Time(std::string time) {
	this->hours = std::string() + time[0] + time[1];
	this->minutes = std::string() + time[3] + time[4];
	this->seconds = std::string() + time[6] + time[7];
}

Time::Time(std::string hours, std::string minutes, std::string seconds) {
	this->hours = hours;
	this->minutes = minutes;
	this->seconds = seconds;
}

bool Time::correctTime(int hours, int minutes, int seconds) {
	return (hours < 24) && (hours >= 0) && (minutes < 60) && (minutes >= 0) && (seconds < 60) && (seconds >= 0);
}

bool Time::correctSequence(int startHours, int startMinutes, int startSeconds, int endHours, int endMinutes, int endSeconds) {
	return correctTime(startHours, startMinutes, startSeconds) && correctTime(endHours, endMinutes, endSeconds) && ((startHours < endHours) || (startHours == endHours && startMinutes < endMinutes) || (startHours == endHours && startMinutes == endMinutes && startSeconds < endSeconds));
}
