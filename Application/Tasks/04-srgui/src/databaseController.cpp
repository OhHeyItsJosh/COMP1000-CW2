#include "databaseController.h"

#include "QMessageBox"
#include <algorithm>

Record* DatabaseController::previousEntry()
{
    // decrement index if it can
    if (m_entryIndex > 0)
        m_entryIndex--;

    return this->getCurrentRecord();
}

Record* DatabaseController::nextEntry()
{
    // increment index if it can
    if (m_entryIndex < m_database.entryCount() - 1)
        m_entryIndex++;

    return this->getCurrentRecord();
}

Record* DatabaseController::setEntry(uint32_t sid)
{
    // find entry index
    int32_t index = this->indexOfRecord(sid);
    if (index == -1)
        return nullptr;

    // set entry index
    m_entryIndex = index;
    return this->getCurrentRecord();
}

Record* DatabaseController::getCurrentRecord()
{
    // return null if there are no records
    if (m_keyList.size() == 0)
        return nullptr;

    return m_database.getRecord(m_keyList[m_entryIndex]);
}

void DatabaseController::createTestDB(const QString& path)
{
    m_database.createTestDB(path.toStdString());
}

bool DatabaseController::importDatabaseFile(const QString& file)
{
    std::string fileStd = file.toStdString();

    // create seperate database object incase database import is unsuccessful
    Database importedDB;
    bool success = importedDB.importFromFile(fileStd);

    if (success)
    {
        // set current database to imported database.
        m_database = importedDB;

        // set state info
        this->createKeyList();
        m_entryIndex = 0;
        m_databasePath = fileStd;
        m_activeDatabase = true;
        m_changesMade = false;
    }

    return success;
}

bool DatabaseController::saveDatabaseFile()
{
    return m_database.exportToFile(m_databasePath);
}

// creates a vector of sid keys from the map so they can be accessed with an index
void DatabaseController::createKeyList()
{
    m_keyList = std::vector<uint32_t>(m_database.entryCount());
    uint32_t index = 0;

    m_database.forEachRecord([&](Record& record, bool last){
        m_keyList[index] = record.sid;
        index++;
    });
}

void DatabaseController::closeDatabase()
{
    m_activeDatabase = false;
}

bool DatabaseController::hasActiveDatabase()
{
    return m_activeDatabase;
}

void DatabaseController::setDirty(bool dirty)
{
    m_changesMade = dirty;
}

// returns whether a change has been made to the database
bool DatabaseController::isDirty()
{
    return m_changesMade && m_activeDatabase;
}

// get the enabled states for the next and back buttons
void DatabaseController::getSwitcherActiveStates(bool& in_prev, bool& in_next)
{
    if (m_entryIndex == 0)
        in_prev = false;
    else
        in_prev = true;

    if (m_entryIndex >= m_keyList.size() - 1)
        in_next = false;
    else
        in_next = true;
}

QString DatabaseController::getDatabaseName()
{
    return QString::fromStdString(m_databasePath).split("/").last();
}

bool DatabaseController::createRecord(uint32_t sid, QString& name)
{
    // abort if record exists
    if (m_database.hasRecord(sid))
        return false;

    // create record and set data
    Record record;
    record.sid = sid;
    record.name = name.toStdString();

    m_database.addRecord(record);
    // regen keylist is easier than finding where to insert key
    this->createKeyList();
    // find the index of the created record and set as active index
    m_entryIndex = this->indexOfRecord(sid);
    return true;
}

void DatabaseController::deleteCurrentRecord()
{
    m_database.removeRecord(m_keyList[m_entryIndex]);

    // remove record from keylist
    m_keyList.erase(m_keyList.begin() + m_entryIndex);
    // decrement current record index if we can
    m_entryIndex = std::max<int32_t>(m_entryIndex - 1, 0);
}

int32_t DatabaseController::indexOfRecord(uint32_t sid)
{
    auto iter =  std::find(m_keyList.begin(), m_keyList.end(), sid);
    if (iter == m_keyList.end())
    {
        return -1;
    }

    return iter - m_keyList.begin();
}
