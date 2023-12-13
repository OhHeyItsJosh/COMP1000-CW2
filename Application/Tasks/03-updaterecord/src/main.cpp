#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <regex>
#include <map>
#include <string>

#include "main.h"
#include "coreUtils.h"
#include "coreModels.h"
/*
 * Updates an EXISTING user in an existing database file
 *
 * The user can pass the following parameters to this application:
 * 
 *  updaterecord -db <database file> -sid <id> [-name <name>] [-phone <phone-number>] [-modulecode <code> -grade <grade>]
 *
 *  updaterecord
 *   Without any parameters, this creates an example database file computing.txt (already done for you)
 *
 *  The -db <database file> parameters are ALWAYS required if you wish to update any records.
 *   If the file does not exist, it should be created (as an empty database)
 * 
 *  When -sid parameter is provided, you must also provide an integer <id>.
 *   The <id> must match a record in the database file.
 * 
 *  The following properties are optional, and may be provided to update data for a given student with id <sid>:
 *
 *   o The student name can be updated with the -name tag. The value <name> is a string, composed of at least two words, separated by spaces. This should replace the existing name
 * 
 *   o The phone number can be added OR updated by specifying the -phone parameter followed by a new phone number. 
 * 
 *   o An individual student grade can be added OR updated using the -modulecode and -grade parameters together.
 *
 * Note that the format of all data items should be consistent with those specified in the previous tasks.
 * The same error checking should also apply.
 *
 * **********************
 * *** VALID EXAMPLES ***
 * **********************
 *
 *
 * Create an empty database file computing.txt
 *  updaterecord -db computing.txt
 * 
 * update the phone number of a record with a student ID 12345
 *  updaterecord -db computing.txt -sid 12345 -phone 00-12-34567
 *
 * update the name and phone number of a record with a student ID 12345
 *  updaterecord -db computing.txt -sid 12345 -phone 00-12-34567 -name Jo Kingly Blunte
 *
 * update (or add) the COMP1001 grade for student with id 12345
 *  updaterecord -db computing.txt -sid 12345 -modulecode COMP1001 -grade 78.4
 *
 * Enrol the student in COMP1001 for student with id 12345
 *  updaterecord -db computing.txt -sid 12345 -modulecode COMP1001
 *
 * **********************
 * *** INVALID EXAMPLES *
 * **********************
 * 
 *  Examples of invalid parameters include:
 *
 *  Missing the database tag
 *   updaterecord -sid 12345
 *
 *  ID does not exist (assuming there is no record with ID 99999)
 *   updaterecord -db computing.txt -sid 99999 -modulecode COMP1001 -grade 78.4
 *
 *  Missing module code
 *   updaterecord -db computing.txt -sid 12345 -grade 78.4
 * 
 *  Spaces in the phone number
 *   updaterecord -db computing.txt -sid 12345 -phone 00 12 34567 -name Jo Kingly Blunte
 *
 *  Name is missing
 *   updaterecord -db computing.txt -sid 12345 -phone 00-12-34567 -name
 *
 *  Name must have at least 2 words
 *   updaterecord -db computing.txt -sid 12345 -phone 00-12-34567 -name Sam
 * 
 * *************
 * *** NOTES ***
 * *************
 * 
 * o The verb update implies you are changing an existing object. The object in this context is a student record.
 *  o You cannot update a student record that does not exist
 *  o You cannot add a student record with update command
 *  o You can update OR add data within an existing record
 *   o For example, if you update a student phone number and no number is currently held on record, 
 *     then in effect you are adding a phone number.
 * o Errors should be communicated with the user
 * o No tag should be repeated (e.g. you cannot specify -sid twice)
 * o Open files should always be closed before the program exits
 * o Code should be consistently indented and commented
 * o Marks are awarded for reuse of code from Tasks A and/or B (where possible)
 * o Marks are awarded for using Object Orientated Programming to make your code more modular and reusable
*/

// Main program here

#define ARG_DB "-db"
#define ARG_SID "-sid"

#define ARG_NAME "-name"
#define ARG_PHONE "-phone"
#define ARG_GRADE "-grade"
#define ARG_MODULE "-modulecode"

int main(int argc, const char *argv[])
{
    Database database;
    if (argc == 1) {
        //Welcome message
        std::cout << "updaterecord (c)2023" << std::endl;

        //Create some test data
        database.createTestDB("computing.txt");

        //Done
        return EXIT_SUCCESS;
    }

    CMDParseResult parsedArgs = CMDUtils::parseArgs(argc, argv, {
        CMDTagParser::tagWithArgument(ARG_DB),
        CMDTagParser::tagWithArgument(ARG_SID),
        CMDTagParser::tagWithMultipleArguments(ARG_NAME, 2, -1),
        CMDTagParser::tagWithArgument(ARG_PHONE),
        CMDTagParser::tagWithArgument(ARG_GRADE),
        CMDTagParser::tagWithArgument(ARG_MODULE),
    });

    if (parsedArgs.hasUnrequestedArgs())
        parsedArgs.logUnrequestedArgs(std::cout);

    // check that db is provided
    CMDTagParserResult* argDb_in = parsedArgs.getResult(ARG_DB);
    if (argDb_in == nullptr || !argDb_in->isValid())
    {
        if (argDb_in) argDb_in->logArgCount(std::cout);
        std::cout << "Please provide a database with '-db <filename>'" << std::endl;
        return EXIT_FAILURE;
    }

    // import the database
    std::string databaseName = argDb_in->getSingletonInput();
    bool importSuccess = database.importFromFile(databaseName);
    if (!importSuccess)
    {
        std::cout << "Provided database could not be loaded, please make sure the file you provided exists and is a valid database file" << std::endl;
        return EXIT_FAILURE;
    }

    // get student ID input
    CMDTagParserResult* argSid_in = parsedArgs.getResult(ARG_SID);
    if (argSid_in == nullptr || !argSid_in->isValid())
    {
        if (argSid_in) argSid_in->logArgCount(std::cout);
        std::cout << "Please provide the SID of the record you would like to update: '-sid <student id>'" << std::endl;
        return EXIT_FAILURE;
    }

    // parse student id
    std::optional<uint32_t> parsedSID = safeParse<std::string, uint32_t>(argSid_in->getSingletonInput(), [](const std::string& input) {
        return stoi(input);
    });
    if (!parsedSID.has_value())
    {
        std::cout << "SID could not be parsed, please make sure it is a valid integer" << std::endl;
        return EXIT_FAILURE;
    }

    // find record for provided sid if it exists
    Record* record = database.getRecord(*parsedSID);
    if (record == nullptr)
    {
        std::cout << "Provided Student ID could not be found in the database, make sure this student exists" << std::endl;
        return EXIT_FAILURE;
    }

    // handle the field changes
    std::vector<RecordField> changedFields;
    bool changeSuccess = handleChanges(*record, parsedArgs, changedFields);
    if (!changeSuccess)
        return EXIT_FAILURE;

    if (changedFields.size() == 0)
    {
        std::cout << "No fields where changed, fields are: '-name <name>', '-phone <phone>', '-modulecode <module code>', '-grade <grade> (modulecode must be present)" << std::endl;
        return EXIT_SUCCESS;
    }

    bool saveSuccess = database.exportToFile(databaseName);
    if (!saveSuccess)
    {
        std::cout << "Error: database file '" << databaseName << "' could not be saved" << std::endl;
        return EXIT_FAILURE;
    }

    // print what got changed
    std::cout << "Successfully updated record!\n" 
        << "Changed field(s): "

        << stringifyList<RecordField>(changedFields, [](const RecordField& field, bool last, std::stringstream& builder) {
            builder << "'" << Record::getRecordName(field) << "'" << (last ? "" : ", ");
        }) << "\n"

        << "For record with sid: '" << record->sid << "'"
        << std::endl;
    return EXIT_SUCCESS;
}



// macro for testing whether arg is valid, takes a pointer to CMDTagParserResult (provided arg must not be null)
//#define DEFAULT_ARG_VALID_CHECK(arg) if (!arg->isValid()) \
//{\
//    arg->logArgCount(std::cout); \
//    return false; \
//}

bool handleChanges(Record& record, CMDParseResult& parsedArgs, std::vector<RecordField>& in_changedFields)
{
    // process name change
    CMDTagParserResult* argName_in = parsedArgs.getResult(ARG_NAME);
    if (argName_in != nullptr)
    {
        ENSURE_ARG_VALID(argName_in, false);
        record.name = defaultStringifyList(argName_in->inputs);
        in_changedFields.push_back(RecordField::NAME);
    }

    // process phone number change
    CMDTagParserResult* argPhone_in = parsedArgs.getResult(ARG_PHONE);
    if (argPhone_in != nullptr)
    {
        ENSURE_ARG_VALID(argPhone_in, false);
        record.phone = argPhone_in->getSingletonInput();
        in_changedFields.push_back(RecordField::PHONE);
    }

    // process changes to grades / modules
    CMDTagParserResult* argModule_in = parsedArgs.getResult(ARG_MODULE);
    CMDTagParserResult* argGrade_in = parsedArgs.getResult(ARG_GRADE);
    if (argModule_in != nullptr || argGrade_in != nullptr)
    {
        // check that a module code is provided, (if it is not, that means only a grade was provided)
        ENSURE_REQUIRED_ARG_VALID(argModule_in, "-module", "A corresponding module must be provided for your grade: '-module <module code>", false);
        in_changedFields.push_back(RecordField::ENROLLMENTS);

        // set the grade depending on whether one was provided, default is -1 if not provided
        float grade;

        if (argGrade_in != nullptr)
        {
            ENSURE_ARG_VALID(argGrade_in, false);

            // attempt to parse grade
            auto gradeParse = safeParse<std::string, float>(argGrade_in->getSingletonInput(), [](const std::string& input) {
                return std::stof(input);
            });
            if (!gradeParse.has_value())
            {
                std::cout << "One or more of the grades provided could not be parsed, please make sure they are valid numbers" << std::endl;
                return false;
            }

            grade = *gradeParse;
            in_changedFields.push_back(RecordField::GRADES);
        }
        else {
            grade = -1;
        }

        record.setEnrollmentAndGrade(argModule_in->getSingletonInput(), grade);
    }

    return true;
}
