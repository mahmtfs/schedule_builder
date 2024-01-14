#include "date.h"

std::string Date::getCurrentDate() {
	char s[20];
	const time_t t = time(0);
	strftime(s, 20, "%Y/%m/%d", localtime(&t));
	return s;
}
