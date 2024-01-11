#include "record.h"
#include <sstream>

#include "../basicUtils.h"

std::map<RecordField, std::string> Record::s_fieldNames = 
{
	{ RecordField::SID, "Student Id" },
	{ RecordField::NAME, "Name" },
	{ RecordField::ENROLLMENTS, "Enrolments" },
	{ RecordField::GRADES, "Grades" },
	{ RecordField::PHONE, "Phone" }
};

//void Record::addEnrollmentAndGrade(const std::string& enrollment, float grade)
//{
//	this->enrollments.push_back(enrollment);
//	this->grades.push_back(grade);
//}

int32_t Record::getEnrollmentIndex(const std::string& enrollment)
{
	auto itr = std::find(this->enrollments.begin(), this->enrollments.end(), enrollment);

	// return -1 if not present
	if (itr == this->enrollments.end())
		return -1;

	return itr - this->enrollments.begin();
}

void Record::setEnrollmentAndGrade(const std::string& enrollment, float grade)
{
	int32_t index = this->getEnrollmentIndex(enrollment);
	if (index == -1)
	{
		// if it is not included
		this->enrollments.push_back(enrollment);
		this->grades.push_back(grade);
	}
	else {
		// if the enrollment exists within the enrollments list
		this->grades[index] = grade;
		return;
	}
}

std::string Record::fieldToString(RecordField field)
{
	switch (field)
	{
	case SID: 
		return std::to_string(this->sid);
	case NAME: 
		return this->name;
	case ENROLLMENTS: 
		return defaultStringifyList<std::string>(this->enrollments);
	case GRADES:
		return defaultStringifyList<float>(this->grades);
	case PHONE:
		return this->phone;
	default:
		return "";
	}
}

#define NO_VALUE "(no value)"

std::string Record::fieldToDisplayString(RecordField field)
{
	// custom stringify function for when a grade is not present (signified by it being -1)
	if (field == RecordField::GRADES)
	{
		if (this->grades.size() == 0)
			return NO_VALUE;

		return stringifyList<float>(this->grades, [](const float& item, bool last, std::stringstream& builder) {
			if (item == -1)
				builder << "(no grade)" << (last ? "" : " ");
			else
				builder << item << (last ? "" : " ");
		});
	}

	std::string stringForField = this->fieldToString(field);
	if (stringForField == "")
		return NO_VALUE;

    return stringForField;
}

std::string Record::getFullDisplayString()
{
	return this->getDisplayStringForFields({ RecordField::SID, RecordField::NAME, RecordField::ENROLLMENTS, RecordField::GRADES, RecordField::PHONE });
}

std::string Record::getDisplayStringForFields(std::vector<RecordField> fields)
{
	return stringifyList<RecordField>(fields, [&](const RecordField& item, bool last, std::stringstream& builder) {
		builder << this->getRecordName(item) << ":\n  " + this->fieldToDisplayString(item) << "\n";
	});
}

void Record::writeTag(std::ostream& stream, const char* tag, const std::string& content)
{
	if (content == "")
		return;

	stream << " " << tag << std::endl
		<< "    " << content << std::endl;
}


void Record::writeData(std::ostream& stream) 
{
	stream << "#RECORD" << std::endl;
	this->writeTag(stream, TAG_SID, this->fieldToString(RecordField::SID));
	this->writeTag(stream, TAG_NAME, this->fieldToString(RecordField::NAME));
	this->writeTag(stream, TAG_ENROLLMENTS, this->fieldToString(RecordField::ENROLLMENTS));
	this->writeTag(stream, TAG_GRADES, this->fieldToString(RecordField::GRADES));
	this->writeTag(stream, TAG_PHONE, this->fieldToString(RecordField::PHONE));
}

std::string& Record::getRecordName(RecordField field)
{
	return s_fieldNames[field];
}
