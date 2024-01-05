#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <vector>
#include <string>
#include "QString"

#include "coreModels.h"

class DatabaseController
{
public:
    DatabaseController() = default;

public:
    Record* previousEntry();
    Record* nextEntry();
    Record* setEntry(uint32_t sid);
    Record* getCurrentRecord();

    bool importDatabaseFile(std::string file);
    bool saveDatabaseFile();
    void closeDatabase();
    bool hasActiveDatabase();
    void setDirty(bool dirty);
    bool isDirty();
    void getSwitcherActiveStates(bool& in_prev, bool& in_next);
    QString getDatabaseName();
    bool createRecord(uint32_t sid, QString& name);
    void deleteCurrentRecord();

private:
    Database m_database;
    std::string m_databasePath;
    bool m_activeDatabase = false;
    bool m_changesMade = false;

    std::vector<uint32_t> m_keyList;
    uint32_t m_entryIndex;

private:
    void createKeyList();
    int32_t indexOfRecord(uint32_t sid);
};

#endif // DATABASECONTROLLER_H
