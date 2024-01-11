#include "scheduleBuilder.h"

ScheduleBuilder* ScheduleBuilder::instance = nullptr;

ScheduleBuilder::ScheduleBuilder(std::string configPath) {
	this->configPath = configPath;
	fillTasks(this->configPath);
}

ScheduleBuilder* ScheduleBuilder::getInstance(std::string configPath) {
	if (instance == nullptr) {
		instance = new ScheduleBuilder(configPath);
	}
	return instance;
}

std::vector<Task*> ScheduleBuilder::getTasks() {
	return tasks;
}

void ScheduleBuilder::addTask(Task* task) {
	tasks.push_back(task);
	rapidxml::file<> xmlFile(this->configPath.c_str());
	rapidxml::xml_document<> doc;
	doc.parse<0x20 | 0x1>(xmlFile.data());
	rapidxml::xml_node<>* node = doc.allocate_node(rapidxml::node_element, doc.allocate_string("task"));
	rapidxml::xml_node<>* name = doc.allocate_node(rapidxml::node_element, "name", doc.allocate_string(task->getName().c_str()));
	rapidxml::xml_node<>* points = doc.allocate_node(rapidxml::node_element, "points", doc.allocate_string(std::to_string(task->getPoints()).c_str()));
	rapidxml::xml_node<>* checked = doc.allocate_node(rapidxml::node_element, "checked", doc.allocate_string((task->isChecked()) ? "true" : "false"));
	node->append_node(name);
	node->append_node(points);
	node->append_node(checked);
	doc.first_node("tasks")->append_node(node);
	std::ofstream outFile(this->configPath);
	outFile << doc;
	outFile.close();
}

void ScheduleBuilder::fillTasks(std::string filePath) {
	this->tasks.clear();
	rapidxml::file<> xmlFile(filePath.c_str());
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	rapidxml::xml_node<>* tasks = doc.first_node("tasks");
	for (rapidxml::xml_node<>* task = tasks->first_node("task"); task; task = task->next_sibling("task")) {
		bool check; 
		std::istringstream(task->first_node("checked")->value()) >> std::boolalpha >> check;
		Task* newTask = new Task(task->first_node("name")->value(), std::stoi(task->first_node("points")->value()), check);
		this->tasks.push_back(newTask);
	}
}

void ScheduleBuilder::check(Task* task) {
	rapidxml::file<> xmlFile(this->configPath.c_str());
	rapidxml::xml_document<> doc;
	doc.parse<0x20 | 0x1>(xmlFile.data());
	rapidxml::xml_node<>* node = doc.first_node(task->getName().c_str());
	node->parent()->first_node("checked")->value((*task->isChecked()) ? "true" : "false");
	std::ofstream outFile(this->configPath);
	outFile << doc;
	outFile.close();
}
