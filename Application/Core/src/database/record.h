#pragma once
#include <vector>
#include <string>
#include <map>
#include <set>

#define TAG_SID "#SID"
#define TAG_NAME "#NAME"
#define TAG_ENROLLMENTS "#ENROLLMENTS"
#define TAG_GRADES "#GRADES"
#define TAG_PHONE "#PHONE"

enum RecordField {
	SID, NAME, ENROLLMENTS, GRADES, PHONE
};

struct Record
{
private:
	//std::map<RecordField, void*> m_fieldBindings;
	static std::map<RecordField, std::string> s_fieldNames;

private:
	std::string fieldToString(RecordField field);

public:
	Record() = default;

public:
	uint32_t sid;
	std::string name;
	std::vector<std::string> enrollments;
	std::vector<float> grades;
	std::string phone;


	void addEnrollmentAndGrade(std::string enrollment, float grade);
	std::string getFullDisplayString();
	std::string getDisplayStringForFields(std::vector<RecordField> fields);

	static std::string& getRecordName(RecordField field);
};