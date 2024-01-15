#pragma once
#include <string>
#include <iostream>
#include "rapidxml.hpp"
#include "rapidxml_iterators.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

class Config {
	std::string path;
	std::string lastVisited;
	std::string dbPath;
	std::string* notes;
	void parseConfig();
public:
	Config(std::string path);
	void setLastVisited(std::string date);
	std::string getLastVisited() { return lastVisited; };
	std::string getDatabasePath() { return dbPath; };
	std::string* getNotes() { return notes; };
	void setNotes(std::string notes);
};
