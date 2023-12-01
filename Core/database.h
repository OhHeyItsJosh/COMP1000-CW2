#pragma once
#include "record.h";
#include <iostream>
#include <map>
#include <sstream>
#include <fstream>
#include <exception>
#include <regex>

class Database
{
private:
	std::vector<Record> m_records;

public:
	Database() = default;

public:
	bool importFromFile(std::string fileName);
};