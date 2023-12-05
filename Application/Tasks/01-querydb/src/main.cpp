#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <regex>
#include <map>
#include <string>

#include "main.h"

/*
 *
 * The user can pass the following parameters to this application:
 *
 * -db <database file>          Specifies the path to the database file (required)
 * -showAll                     Writes all records to the terminal
 * -sid <student ID> [-n|-g|-p] Writes the record with a specific student ID (integer).
 *                              By default, this displays the complete record
 *                              Add one of the following to reduce the information
 *                                 -n       Just display the name
 *                                 -g       Just display the mode codes and grades
 *                                 -p       Just display the phone number
 *
 * ****************
 * *** EXAMPLES ***
 * ****************
 * querydb                                  Creates an example database computing.txt (done for you)
 * querydb -db computing.txt -showAll       Displays all records in the database computing.txt (done for you)
 * querydb -db computing.txt -sid 12345     Displays the complete record for student with ID 12345 (done for you)
 *
 * ********* The above three examples are already implemented, but not fully tested *********
 *
 * For the -sid option, you can further narrow down the information displayed (not yet implemented)
 *
 * ****************
 * *** EXAMPLES ***
 * ****************
 * 
 * querydb -db computing.txt -sid 12345 -n      Displays only the name for student with ID 12345, or an error if not found
 * querydb -db computing.txt -sid 12345 -g      Displays only the module codes and grades for student with ID 12345, or an error if not found
 * querydb -db computing.txt -sid 12345 -p      Displays only the phone number for student with ID 12345, or an error if not found
 * querydb -db computing.txt -sid 12345 -n -p   Display both the name and the phone number for student with ID 12345
 * 
 * *************
 * *** NOTES ***
 * *************
 * 
 * o You should test the existing application first and verify which of the above is already complete.
 * o The -n, -g and -p switches have not yet been implemented (this is your task).
 * o You need to handle error conditions, and inform the user appropriately.
 * o Errors should be communicated with the user.
 * o No tag should be repeated (e.g. you cannot specify -sid twice).
 * o Open files should always be closed before the program exits.
 * o You do not have to use Object Orientated Programming in this task, but it is recommended.
 * o Try to write your code such that is can be reused in other tasks.
 * o Code should be consistently indented and commented
*/

#define ARG_DB "-db"
#define ARG_SHOWALL "-showAll"
#define ARG_SID "-sid"
#define ARG_SHOWNAME "-n"
#define ARG_SHOWGRADES "-g"
#define ARG_SHOWPHONE "-p"

int main(int argc, const char *argv[])
{
    // create the database object
    Database database;
    
    // if no args are provided
    if (argc == 1) {
        //Welcome message
        std::cout << "querydb (c)2023" << std::endl;

        //Create some test data
        database.createTestDB("computing.txt");

        //Done
        return EXIT_SUCCESS;
    }

    using namespace CMDUtils;

    // parse the args, takes a list of parsers where CMDTagParser({tagName}, required
    // uses custom tag parsing system I built for this project.
    CMDParseResult parsedArgs = CMDUtils::parseArgs(argc, argv, {
        CMDTagParser::tagWithArgument(ARG_DB),
        CMDTagParser(ARG_SHOWALL),
        CMDTagParser::tagWithArgument(ARG_SID),
        CMDTagParser(ARG_SHOWNAME),
        CMDTagParser(ARG_SHOWGRADES),
        CMDTagParser(ARG_SHOWPHONE),
    });

    //Scan command line for -db switch
    CMDTagParserResult* db_in = parsedArgs.getResult(ARG_DB);
    if (db_in == nullptr || !db_in->isValid())
    {
        std::cout << "Please provide a database with -db <filename>" << std::endl;
        return EXIT_FAILURE;
    }

    // import the database
    std::string dbName = db_in->value;
    bool importSuccessful = database.importFromFile(dbName);
    if (!importSuccessful)
    {
        std::cout << "Provided database could not be loaded, please make sure the file you provided exists and is a valid database file" << std::endl;
        return EXIT_FAILURE;
    }

    //*******************************
    //Option to display data ALL DATA
    //*******************************

    bool showALl = parsedArgs.hasResult(ARG_SHOWALL);
    if (showALl) 
    {
        std::stringstream outputBuilder;

        const std::string spacer = "----------\n";
        outputBuilder << spacer;

        database.forEachRecord([&](Record& record, bool isLast) 
        {
            outputBuilder << record.getFullDisplayString();
            outputBuilder << spacer;
        });

        std::cout << outputBuilder.str() << std::endl;
        return EXIT_SUCCESS;
    }

    //**************************************************************
    //Option to display data from one record with a given student ID
    //**************************************************************
    CMDTagParserResult* sid_in = parsedArgs.getResult(ARG_SID);
    if (sid_in == nullptr || !sid_in->isValid())
    {
        std::cout << "Please provide a student ID after -sid" << std::endl;
        return EXIT_FAILURE;
    }
    
    // attempt to parse the student id
    uint32_t studentId;

    try {
        studentId = stoi(sid_in->value);

    }
    catch (std::exception e) {
        std::cout << "Student ID could not be parsed, make sure it is an integer" << std::endl;
        return EXIT_FAILURE;
    }

    Record* record = database.getRecord(studentId);
    
    // check that the student exists
    if (record == nullptr)
    {
        std::cout << "The provided student ID is not in the database" << std::endl;
        return EXIT_FAILURE;
    }

    // ********************************************
    // ** handle specifiers if they are provided **
    // ********************************************
    bool nameSpecifier = parsedArgs.hasResult(ARG_SHOWNAME);
    bool gradesSpecifier = parsedArgs.hasResult(ARG_SHOWGRADES);
    bool phoneSpecifier = parsedArgs.hasResult(ARG_SHOWPHONE);

    // if a specifier is provided
    if (nameSpecifier || gradesSpecifier || phoneSpecifier)
    {
        std::vector<RecordField> displayFields = getFieldsFromFlags(nameSpecifier, gradesSpecifier, phoneSpecifier);
        std::cout << record->getDisplayStringForFields(displayFields) << std::endl;
    }
    // display all fields if no specifier is provided
    else
        std::cout << record->getFullDisplayString() << std::endl;

    return EXIT_SUCCESS;
}

std::vector<RecordField> getFieldsFromFlags(bool showName, bool showGrades, bool showPhone)
{
    std::vector<RecordField> fields;
    
    if (showName)
        fields.push_back(RecordField::NAME);

    if (showGrades) {
        fields.push_back(RecordField::GRADES);
        fields.push_back(RecordField::ENROLLMENTS);
    }

    if (showPhone)
        fields.push_back(RecordField::PHONE);

    return fields;
}
