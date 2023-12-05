#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <regex>
#include <map>
#include <string>

#include "core.h"

//See bottom of main
int findArg(int argc, char *argv[], std::string pattern);


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
    CMDParseResult parsedArgs = CMDUtils::parseArgs(argc, argv, {
        CMDTagParser::tagWithArgument(ARG_DB),
        CMDTagParser(ARG_SHOWALL),
        CMDTagParser::tagWithArgument(ARG_SID)
        });

    //Scan command line for -db switch
    CMDTagParserResult* db_in = parsedArgs.getResult(ARG_DB);
    if (db_in == nullptr || !db_in->isValid())
    {
        std::cout << "Please provide a database with -db <filename>" << std::endl;
        return EXIT_FAILURE;
    }


    //*******************************
    //Option to display data ALL DATA
    //*******************************

    std::string dbName = db_in->value;
    bool importSuccessful = database.importFromFile(dbName);
    if (!importSuccessful)
    {
        std::cout << "Databse could not be imported";
        return EXIT_FAILURE;
    }

    CMDTagParserResult* showAll_in = parsedArgs.getResult(ARG_SHOWALL);
    if (showAll_in != nullptr) 
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
    /*    for (Record& r : db) {
            printRecord(r);
            cout << endl;
        }*/
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

    try {
        uint32_t studentId = stoi(sid_in->value);
        Record* record = database.getRecord(studentId);

        std::cout << record->getFullDisplayString() << std::endl;
    }
    catch (std::exception e) {
        std::cout << "Student ID could not be parsed, make sure it is an integer" << std::endl;
        return EXIT_FAILURE;
    }
    //if (p) {

    //    //Did they provide a SID?
    //    string strID = argv[p+1];

    //    //Try to convert to a number
    //    int sid;
    //    try {
    //        //Extract student ID as integer - can throw an exception if not an integer
    //        sid = stoi(strID);

    //        //Search for record with this ID
    //        bool found = false;
    //        for (Record& r : db) {
    //            if (r.SID == sid) {
    //                //Display
    //                printRecord(r);
    //                //Flag that a match was found
    //                found = true;
    //                break;
    //            }
    //        }

    //        //In the event a match was not found, tell the user
    //        if (!found) {
    //            cout << "No record with SID=" << strID << " was found" << endl;
    //        }

    //    } catch (exception e) {
    //        cout << "Please provide a student ID as an integer" << endl;
    //        return EXIT_FAILURE;
    //    } //End try

    //} //End if

    //**************************************************************
    //You could continue here :)
    //**************************************************************

    return EXIT_SUCCESS;
}