#include "config.h"

Config::Config(std::string path) {
	this->path = path;
	parseConfig();
}

void Config::parseConfig() {
	rapidxml::file<> xmlFile(path.c_str());
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	rapidxml::xml_node<>* root = doc.first_node("config");
	lastVisited = root->first_node("last-visited")->value();
	dbPath = root->first_node("db-path")->value();
	notes = new std::string(root->first_node("notes")->value());
}

void Config::setLastVisited(std::string date) {
	lastVisited = date;
	rapidxml::file<> xmlFile(path.c_str());
	rapidxml::xml_document<> doc;
	doc.parse<0x20 | 0x1>(xmlFile.data());
	rapidxml::xml_node<>* lastVisitedNode = doc.first_node("config")->first_node("last-visited");
	lastVisitedNode->value(date.c_str());
	std::ofstream outFile(path);
	outFile << doc;
	outFile.close();
}

void Config::setNotes(std::string notes) {
	rapidxml::file<> xmlFile(path.c_str());
	rapidxml::xml_document<> doc;
	doc.parse<0x20 | 0x1>(xmlFile.data());
	rapidxml::xml_node<>* notesNode = doc.first_node("config")->first_node("notes");
	notesNode->value(notes.c_str());
	std::ofstream outFile(path);
	outFile << doc;
	outFile.close();
}
