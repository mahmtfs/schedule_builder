#pragma once
#include <string>
#include <vector>
#include "../../libs/rapidxml/rapidxml.hpp"
//#include "rapidxml.hpp"
#include "../../libs/rapidxml/rapidxml_iterators.hpp"
#include "../../libs/rapidxml/rapidxml_utils.hpp"
#include "../../libs/rapidxml/rapidxml_print.hpp"

class __declspec(dllexport) Config {
	std::string path;
	std::string lastVisited;
	std::string dbPath;
	void parseConfig();
public:
	Config(std::string path);
	void setLastVisited(std::string date);
	std::string getLastVisited() { return lastVisited; };
	std::string getDatabasePath() { return dbPath; };
};
