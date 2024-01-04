#include "databaseController.h"

#include "QMessageBox"

Record* DatabaseController::previousEntry()
{
    if (m_entryIndex > 0)
        m_entryIndex--;

    return m_database.getRecord(m_keyList[m_entryIndex]);
}

Record* DatabaseController::nextEntry()
{
    if (m_entryIndex < m_database.entryCount() - 1)
        m_entryIndex++;

    return m_database.getRecord(m_keyList[m_entryIndex]);
}

Record* DatabaseController::getCurrentRecord()
{
    if (m_keyList.size() == 0)
        return nullptr;

    return m_database.getRecord(m_keyList[m_entryIndex]);
}

bool DatabaseController::importDatabaseFile(std::string file)
{
    Database importedDB;
    bool success = importedDB.importFromFile(file);

    if (success)
    {
        // re-create database info
        m_database = importedDB;
        this->createKeyList();
        m_entryIndex = 0;
        m_databasePath = file;
        m_activeDatabase = true;
    }

    return success;
}

bool DatabaseController::saveDatabaseFile()
{
    return m_database.exportToFile(m_databasePath);
}

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
    // maybe do some other stuff in the future such as closing a file stream
}

bool DatabaseController::hasActiveDatabase()
{
    return m_activeDatabase;
}

void DatabaseController::setDirty(bool dirty)
{
    m_changesMade = dirty;
}

bool DatabaseController::isDirty()
{
    return m_changesMade;
}
