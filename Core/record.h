#pragma once
#include <vector>
#include <string>
#include <functional>
#include <map>
#include <set>
#include "serialisation.h"

#define TAG_SID "#SID"
#define TAG_NAME "#NAME"
#define TAG_ENROLLMENTS "#ENROLLMENTS"
#define TAG_GRADES "#GRADES"
#define TAG_PHONE "#PHONE"

struct Record
{
	uint32_t sid;
	std::string name;
	std::vector<std::string> enrollments;
	std::vector<float> grades;
	std::string phone;

	void addEnrollmentAndGrade(std::string enrollment, float grade);
};