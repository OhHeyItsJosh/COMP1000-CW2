#pragma once
#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include <exception>
#include <regex>
#include "functional"

#include "record.h"

class Database
{
private:
	std::map<uint32_t, Record> m_records;

public:
	Database() = default;

public:
	bool importFromFile(std::string fileName);
	bool exportToFile(std::string fileName);
	void createTestDB(std::string name);
    uint32_t entryCount();

	bool hasRecord(uint32_t sid);
	Record* getRecord(uint32_t sid);
	void addRecord(Record& record);
    void removeRecord(uint32_t sid);
	void forEachRecord(std::function<void(Record&, bool)> callback);
};
