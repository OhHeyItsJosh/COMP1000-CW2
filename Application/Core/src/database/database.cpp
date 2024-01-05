#include <functional>

#include "database.h"
#include "testDB.h"

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

    //Locals used for navigating the database 
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
                    if (nextRecord.sid > 0)
                        m_records.emplace(nextRecord.sid, nextRecord);

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
            m_records.emplace(nextRecord.sid, nextRecord);
        }

        //Close the file - we are done reading it. Everything is now in the db vector
        readStream.close();
        return true;
    }
    catch (std::exception e) {
        //Many things could go wrong, so we catch them here, tell the user and close the file (tidy up)
        readStream.close();
        std::cout << "Error reading data" << std::endl;
        std::cerr << e.what() << std::endl;
        return false;
    } //end try

}

bool Database::exportToFile(std::string fileName)
{
    std::ofstream writeStream(fileName);
    if (!writeStream.is_open())
        return false;

    this->forEachRecord([&](Record& record, bool last)
        {
            record.writeData(writeStream);
            if (!last) {
                writeStream << "\n";
            }
        }
    );

    writeStream.close();
    return true;
}

void Database::createTestDB(std::string name)
{
    //Does the file exist?
    std::ifstream ip(name);
    if (ip.is_open()) {
        std::cout << name << " exists" << std::endl;
        ip.close();
        return;
    }

    //Create some test data
    std::cout << "Creating starter database for testing" << std::endl;
    std::ofstream op(name);
    op << TESTSTR;
    op.close();

}

uint32_t Database::entryCount()
{
    return m_records.size();
}

bool Database::hasRecord(uint32_t sid)
{
    return m_records.find(sid) != m_records.end();
}

Record* Database::getRecord(uint32_t sid)
{
    bool hasRecord = m_records.find(sid) != m_records.end();
    if (!hasRecord)
        return nullptr;

    return &m_records.at(sid);
}

void Database::addRecord(Record& record)
{
    m_records.emplace(record.sid, std::move(record));
}

void Database::removeRecord(uint32_t sid)
{
    m_records.erase(sid);
}

void Database::forEachRecord(std::function<void(Record&, bool)> callback)
{
    uint32_t count = 0;
    for (auto& iter : m_records)
    {
        callback(iter.second, count+1 == m_records.size());
        count++;
    }
}


