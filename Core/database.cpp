#include "database.h"
#include <functional>

using InputParser = std::function<void(Record&, std::string&)>;

template<typename T>
std::vector<T> parseVectorFromString(std::string& str, std::function<T(std::string&)> parse)
{
    std::vector<T> results;
    std::stringstream splitStream(str);
    std::string word;

    while (!splitStream.eof())
    {
        splitStream >> word;
        results.push_back(parse(word));
    }

    return results;
}

enum DBReadState
	{ START, NEXTTAG, READTAG };

bool Database::importFromFile(std::string fileName)
{

    //Open database file
    std::ifstream readStream(fileName);
    if (!readStream.is_open()) {
        return false;
    }

    // the sate for the state machine
    DBReadState state = DBReadState::START;

    //Locals used for navigating the database file
    Record nextRecord;
    
    // parsers for parsing each tag
    std::map<std::string, std::function<void(std::string&)>> tagParsers = {
        {TAG_SID, [&](std::string& input) { nextRecord.sid = stoi(input); }},
        {TAG_NAME, [&](std::string& input) { nextRecord.name = input; }},
        {TAG_PHONE, [&](std::string& input) { nextRecord.phone = input; }},
        {TAG_GRADES, [&](std::string& input) {
            nextRecord.grades = parseVectorFromString<float>(input, [](std::string& grade) { return std::stof(grade); });
        }},
        {TAG_ENROLLMENTS, [&](std::string& input) {
            nextRecord.enrollments = parseVectorFromString<std::string>(input, [](std::string& grade) { return grade; });
        }}
    };

    //*********
    //Main loop
    //*********

    std::string nextLine;
    std::string activeTag;

    try
    {
        //Read the next line (loop exits on end of file)
        while (std::getline(readStream, nextLine))
        {
            // Remove leading spaces
            // Replace "start of line (^) followed by any number of trailing spaces (' +')" with with nothing ""
            std::string nextStr = std::regex_replace(nextLine, std::regex("^ +"), "");

            //Skip blank lines
            if (nextStr.empty()) continue;

            //Enter "state machine" - study this carefully - it's a really useful "pattern"
            switch (state)
            {
            case START:
                //We begin here - the first non-blank line MUST start with "#RECORD"
                if (nextStr != "#RECORD") {
                    //The first list MUST simply read #RECORD
                    throw("Expected #RECORD as first tag");
                }

                nextRecord = { 0 };

                state = NEXTTAG;
                break;

            case NEXTTAG:
                if (nextStr == "#RECORD")
                {
                    // begin new record
                    m_records.push_back(nextRecord);
                    nextRecord = { 0 };
                    break;
                }

                activeTag = nextStr;
                state = READTAG;
                break;

            case READTAG:
                // parse the data if it exists
                if (tagParsers.count(activeTag))
                {
                    tagParsers[activeTag](nextStr);
                    state = NEXTTAG;
                }

                break;
            } //End Switch

        } //End while

        //The loop above may exit before pushing the last record into db
        if (nextRecord.sid > 0) {
            m_records.push_back(nextRecord);
        }

        //Close the file - we are done reading it. Everything is now in the db vector
        readStream.close();

    }
    catch (std::exception e) {
        //Many things could go wrong, so we catch them here, tell the user and close the file (tidy up)
        readStream.close();
        std::cout << "Error reading data" << std::endl;
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    } //end try

}
