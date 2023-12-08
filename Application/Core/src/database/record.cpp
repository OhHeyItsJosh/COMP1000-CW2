#include "record.h"
#include <sstream>

#include "../basicUtils.h"

std::map<RecordField, std::string> Record::s_fieldNames = 
{
	{ RecordField::SID, "Student Id" },
	{ RecordField::NAME, "Name" },
	{ RecordField::ENROLLMENTS, "Enrollments" },
	{ RecordField::GRADES, "Grades" },
	{ RecordField::PHONE, "Phone" }
};

void Record::addEnrollmentAndGrade(std::string enrollment, float grade)
{
	this->enrollments.push_back(enrollment);
	this->grades.push_back(grade);
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

std::string Record::getFullDisplayString()
{
	return this->getDisplayStringForFields({ RecordField::SID, RecordField::NAME, RecordField::ENROLLMENTS, RecordField::GRADES, RecordField::PHONE });
}

std::string Record::getDisplayStringForFields(std::vector<RecordField> fields)
{
	return stringifyList<RecordField>(fields, [&](const RecordField& item, bool last, std::stringstream& builder) {
		builder << this->getRecordName(item) << ":\n  " + this->fieldToString(item) << "\n";
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