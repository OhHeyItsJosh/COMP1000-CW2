#include "databaseController.h"

#include "QMessageBox"
#include <algorithm>

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

Record* DatabaseController::setEntry(uint32_t sid)
{
    int32_t index = this->indexOfRecord(sid);
    if (index == -1)
        return nullptr;

    m_entryIndex = index;
    return this->getCurrentRecord();
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
    if (m_database.hasRecord(sid))
        return false;

    Record record;
    record.sid = sid;
    record.name = name.toStdString();

    m_database.addRecord(record);
    this->createKeyList();
    m_entryIndex = this->indexOfRecord(sid);
    return true;
}

void DatabaseController::deleteCurrentRecord()
{
    m_database.removeRecord(m_keyList[m_entryIndex]);

    m_keyList.erase(m_keyList.begin() + m_entryIndex);
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
