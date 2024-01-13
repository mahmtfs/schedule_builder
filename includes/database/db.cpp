#include "db.h"

DataBase* DataBase::instance = nullptr;

DataBase::DataBase(std::string path) {
	char* err;
	sqlite3_open(path.c_str(), &db);
	std::string query = "CREATE TABLE IF NOT EXISTS tasks(id INTEGER PRIMARY KEY, task_name VARCHAR(50), points INT, task_time TEXT, checked INT);";
	if (sqlite3_exec(db, query.c_str(), NULL, NULL, &err) != SQLITE_OK) {
		std::cout << err << "\n";
	}
}

DataBase* DataBase::getInstance(std::string path) {
	if (instance == nullptr) {
		instance = new DataBase(path);
	}
	return instance;
}

std::vector<Task*> DataBase::getTasks() {
	std::vector<Task*> tasks;
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(db, "SELECT id, task_name, points, task_time, checked FROM tasks ORDER BY task_time ASC;", -1, &stmt, NULL);
	while (sqlite3_step(stmt) != SQLITE_DONE) {
		int id = sqlite3_column_int(stmt, 0);
		const unsigned char* name = sqlite3_column_text(stmt, 1);
		int points = sqlite3_column_int(stmt, 2);
		const unsigned char* time = sqlite3_column_text(stmt, 3);
		bool checked = sqlite3_column_int(stmt, 4);
		Task* task = new Task((char*)name, points, new Time((char*)time), checked);
		task->setID(id);
		tasks.push_back(task);
	}
	return tasks;
}

void DataBase::addTask(Task* task) {
	char* err;
	std::string query = "INSERT INTO tasks (task_name, points, task_time, checked) VALUES('"+task->getName()+"', "+std::to_string(task->getPoints())+", '"+task->getTime()->getTime()+ "', " + std::to_string(int(*(task->isChecked()))) + ");";
	if (sqlite3_exec(db, query.c_str(), NULL, NULL, &err) != SQLITE_OK) {
		std::cout << err << "\n";
	}
	task->setID(sqlite3_last_insert_rowid(db));
}

void DataBase::deleteTask(Task* task) {
	char* err;
	std::string query = "DELETE FROM tasks WHERE id="+std::to_string(task->getID())+";";
	if (sqlite3_exec(db, query.c_str(), NULL, NULL, &err) != SQLITE_OK) {
		std::cout << err << "\n";
	}
}

void DataBase::updateTaskCheck(Task* task) {
	char* err;
	std::string query = "UPDATE tasks SET checked="+std::to_string(int(*(task->isChecked())))+" WHERE id="+std::to_string(task->getID())+";";
	if (sqlite3_exec(db, query.c_str(), NULL, NULL, &err) != SQLITE_OK) {
		std::cout << err << "\n";
	}
}
