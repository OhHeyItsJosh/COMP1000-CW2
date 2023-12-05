#pragma once
#include "record.h"
#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include <exception>
#include <regex>
#include "functional"

class Database
{
private:
	std::map<uint32_t, Record> m_records;

public:
	Database() = default;

public:
	bool importFromFile(std::string fileName);
	void createTestDB(std::string name);
	Record* getRecord(uint32_t sid);
	void forEachRecord(std::function<void(Record&, bool)> callback);
};