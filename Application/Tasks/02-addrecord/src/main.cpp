#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <regex>
#include <map>
#include <string>

#include "main.h"

/*
 * Adds a NEW user to the end of an existing database file or the start of a new file
 *
 * The user can pass the following parameters to this application:
 *
 * addrecord -db <database file> [-sid <id> -name <name> [-phone <phone-number>] [-modulecodes <code1> <code2> ... <codeN> [-grades <grade1> <grade2> ... <gradeM> ]]]
 *
 * addrecord
 *  Without any parameters, this creates an example database file computing.txt (already done for you)
 *
 * The -db <database file> parameters are ALWAYS required if you wish to add a record.
 *  If the file does not exist, it should be created (as an empty database)
 * 
 * Where -sid is provided.
 *    • A numerical student ID <id> MUST also be provided.
 *       o <id> must be an unsigned integer. 
 *       o <id> must be unique. If another record in the database has the same value, an error should be reported.
 * 
 * Where -sid is provided, the following MUST also be included:
 *    • The tag -name and a student name <name>.
 *       o <name> is a full name, so will comprise only alphabetical characters and spaces.
 *       o <name> must contain at least two words separated by spaces.
 * 
 * Where -sid is provided, the following MAY also be included:
 *    • The tag -phone followed by a phone number.
 *      o <phone-number> is a string with no spaces.
 *    • The tag -modulecodes, followed by a list of module codes.
 *       o Each module code is a single alpha-numeric word. It cannot contain any symbols.
 *       o Each module code is separated by a space.
 *       o There must be at least one module code provided.
 *    • IF a -modulecodes tag is provided, the following MAY also be provided:
 *       o The tag -grades, followed by a list of grades.
 *       o Each grade must be a fractional number.
 *       o Each grade must be separated by a space.
 *       o The number of grades MUST match the number of module codes.
 *       o You cannot use -grades tag without an accompanying -modulecodes tag
 * 
 *
 * **********************
 * *** VALID EXAMPLES ***
 * **********************
 *
 * Create an empty database file computing.txt
 *  addrecord -db computing.txt
 * 
 * Add a record with a student ID <id> and name <name>, where <id> is an integer and name <name> is a string (space separated)
 *  addrecord -db computing.txt -sid 12345 -name Sam Eold
 *
 * This version also includes the phone number <phone>, where <phone> is a string with no spaces
 *  addrecord -db computing.txt -sid 12345 -name Sam Eold -phone 44-1234-456123
 *
 * This version includes the modules the student is enrolled on
 *  addrecord -db computing.txt -sid 13579 -name Les Sismore -modulecodes COMP101 COMP110 COMP123 COMP145 COMP165
 *
 * This includes the grades as well as the module codes. The number of grades must equal the number of codes or an error is displayed
 *  addrecord -db computing.txt -sid 24680 -name Jo King -modulecodes COMP101 COMP110 COMP123 COMP145 COMP165 -grades 40.5 55.6 35.7 67.5 80.1
 *
 *
 * *************************************
 * *** EXAMPLES OF INVALID ARGUMENTS ***
 * *************************************
 *
 *  Missing the database filename
 *   addrecord -db -sid 12345 -name Sam Eold
 *
 *  Name is missing
 *   addrecord -db computing.txt -sid 12345
 *
 *  -sid is missing
 *   addrecord -db computing.txt -name Sam Eold
 *
 *  <id> is not an integer
 *   addrecord -db computing.txt -sid a2345 -name Sam Eold
 *
 *  phone number cannot contain spaces
 *   addrecord -db computing.txt -sid 12345 -name Sam Eold -phone 44 01234 456123
 *
 *  grades cannot be provided without module codes
 *   addrecord -db computing.txt -sid 24680 -name Jo King -grades 40.5 55.6 35.7 67.5 80.1
 *
 *  Number of module codes and grades do not match
 *  addrecord -db computing.txt -sid 24680 -name Jo King -modulecodes COMP101 COMP110 -grades 40.5 55.6 35.7
 *
 * *************
 * *** NOTES ***
 * *************
 * 
 * o The verb ADD implies you are created a new object. The object in this context is a student record.
 *  o You cannot add a student record that already exists
 *  o To add data to a student record that already exist, see task C
 * 
 * o Errors should be communicated with the user
 * o No tag should be repeated (e.g. you cannot specify -sid twice)
 * o New records should be added to the end of the database file (known as appending). See the labs on FileIO and ios::app
 * o Open files should always be closed before the program exits
 * o You may copy or (better) reuse code from Task A where possible
 * o Code should be consistently indented and commented
*/

// Main program here - you may shared / reuse as much code as you like with other projects included in this starter code

using namespace CMDUtils;

#define ARG_DB "-db"
#define ARG_SID "-sid"

#define ARG_NAME "-name"
#define ARG_PHONE "-phone"
#define ARG_GRADES "-grades"
#define ARG_MODULES "-modulecodes"

int main(int argc, const char *argv[])
{
    Database database;

    if (argc == 1) {
        //Welcome message
        std::cout << "addrecord (c)2023" << std::endl;

        //Create some test data
        database.createTestDB("computing.txt");
        return EXIT_SUCCESS;
    }

    // specify the tags I want to parse
    CMDParseResult parserResult = CMDUtils::parseArgs(argc, argv, {
        CMDTagParser::tagWithArgument(ARG_DB),
        CMDTagParser::tagWithArgument(ARG_SID),
        CMDTagParser::tagWithMultipleArguments(ARG_NAME, 2, -1),
        CMDTagParser::tagWithArgument(ARG_PHONE),
        CMDTagParser::tagWithMultipleArguments(ARG_GRADES, 1, -1),
        CMDTagParser::tagWithMultipleArguments(ARG_MODULES, 1, -1)
    });

    if (parserResult.hasUnrequestedArgs())
        parserResult.logUnrequestedArgs(std::cout);

    // get the database input
    CMDTagParserResult* argDb_in = parserResult.getResult(ARG_DB);
    ENSURE_REQUIRED_ARG_VALID(argDb_in, "Database", "'-db <filename>'", EXIT_FAILURE);

    // import the database
    std::string databaseName = argDb_in->getSingletonInput();
    bool importSuccess = database.importFromFile(databaseName);
    if (!importSuccess)
    {
        database.exportToFile(databaseName);
        std::cout << "Provided database could not be loaded, a blank database '" << databaseName << "' has been created" << std::endl;
    }

    // attempt to parse the record from the inputs
    std::optional<Record> createdRecord = parseRecordInput(parserResult, database);
    if (!createdRecord.has_value())
        return EXIT_FAILURE;

    // add the record to the database and write the database to a file
    database.addRecord(*createdRecord);
    database.exportToFile(databaseName);

    std::cout << "Successfully added record with sid '" << createdRecord->sid << "' to '" << databaseName << "'" << std::endl;
    return EXIT_SUCCESS;
}



// macro to make sure an optional has a value
#define ENSURE_HASVALUE(optional, name, hint) if (!optional.has_value()) { printf("%s could not be parsed: %s", name, hint); return std::nullopt; }

std::optional<Record> parseRecordInput(CMDUtils::CMDParseResult& parsedArgs, Database& database)
{
    // ensure required parameters sid and name
    CMDTagParserResult* argSid_in = parsedArgs.getResult(ARG_SID);
    CMDTagParserResult* argName_in = parsedArgs.getResult(ARG_NAME);

    ENSURE_REQUIRED_ARG_VALID(argSid_in, "Student Id", "'-sid <student id>'", std::nullopt);
    ENSURE_REQUIRED_ARG_VALID(argName_in, "Name", "'-name <student name>'", std::nullopt);
    
    // create the record
    Record record;

    // parse the arguments
    auto studentId = safeParse<std::string, uint32_t>(argSid_in->getSingletonInput(), [](const std::string& id) {
        return stoi(id);
    });
    ENSURE_HASVALUE(studentId, "Student Id", "Must be a number");

    // check if the sid is already being used
    if (database.hasRecord(*studentId))
    {
        std::cout << "A record with the sid '" << *studentId << "' already exists within this database" << std::endl;
        return std::nullopt;
    }

    record.sid = *studentId;
    record.name = defaultStringifyList<std::string>(argName_in->inputs);

    CMDTagParserResult* argPhone_in = parsedArgs.getResult(ARG_PHONE);
    CMDTagParserResult* argGrades_in = parsedArgs.getResult(ARG_GRADES);
    CMDTagParserResult* argEnrollments_in = parsedArgs.getResult(ARG_MODULES);

    // set phone if provided
    if (argPhone_in != nullptr)
    {
        ENSURE_ARG_VALID(argPhone_in, std::nullopt);
        record.phone = argPhone_in->getSingletonInput();
    }

    // parse grades / moduleCodes if provided
    if (argGrades_in != nullptr || argEnrollments_in != nullptr)
    {
        // check that a module code is provided, (if it is not, that means only a grade was provided)
        ENSURE_REQUIRED_ARG_VALID(argEnrollments_in, "-modulecodes", "grades cannot be provided without an accompanied module code '-modulecodes <module codes...>'", std::nullopt);


        std::vector<float> grades;

        // get grades if they are provided. If not, initialise them to -1 (no grade)
        if (argGrades_in != nullptr)
        {
            ENSURE_ARG_VALID(argGrades_in, std::nullopt);

            // safely parse the grades to vector of float
            auto gradesParse = safeParse<std::vector<std::string>, std::vector<float>>(argGrades_in->inputs, [](const std::vector<std::string>& input) {
                // map vector of string to vector of float
                return mapVector<std::string, float>(input, [](std::string& item) {
                    return std::stof(item);
                });
            });

            // check that grades and enrollments are the same size
            if (argGrades_in->inputs.size() != argEnrollments_in->inputs.size())
            {
                std::cout << "You must provide a module code for each grade and vice versa." << std::endl;
                return std::nullopt;
            }

            ENSURE_HASVALUE(gradesParse, "Grades", "All provides entries must be numbers");
            grades = *gradesParse;
        }
        else {
            grades.resize(argEnrollments_in->inputs.size());
            std::fill(grades.begin(), grades.end(), -1);
        }

        // set data
        record.enrollments = argEnrollments_in->inputs;
        record.grades = grades;
    }

    return record;
}
