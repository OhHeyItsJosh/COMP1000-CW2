#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <vector>
#include <string>

#include "coreModels.h"

class DatabaseController
{
public:
    DatabaseController() = default;

public:
    Record* previousEntry();
    Record* nextEntry();
    Record* getCurrentRecord();

    bool importDatabaseFile(std::string file);
    bool saveDatabaseFile();
    void closeDatabase();
    bool hasActiveDatabase();
    void setDirty(bool dirty);
    bool isDirty();

private:
    Database m_database;
    std::string m_databasePath;
    bool m_activeDatabase = false;
    bool m_changesMade = false;

    std::vector<uint32_t> m_keyList;
    uint32_t m_entryIndex;

private:
    void createKeyList();
};

#endif // DATABASECONTROLLER_H
