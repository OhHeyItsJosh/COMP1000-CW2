#include "record.h"

void Record::addEnrollmentAndGrade(std::string enrollment, float grade)
{
	this->enrollments.push_back(enrollment);
	this->grades.push_back(grade);
}