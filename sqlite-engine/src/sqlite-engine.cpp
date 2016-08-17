#include "sqlite-engine.h"

#include "sqlite3.h"
#include <sys/stat.h>

namespace hb
{
namespace sqlite
{
namespace
{
inline bool file_exists(const std::string& name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}
}

bool SqliteEngine::IsDbExists(const std::string& path)
{
    return file_exists(path);
}

bool SqliteEngine::CreateNewDb(const std::string& path)
{
    return false;
}

SqliteEngine::SqliteEngine(const std::string& path)
    : m_pathToDatabase(path)
{
    if (!IsDbExists(path))
    {
        CreateNewDb(path);
    }

    if (!IsDbExists(path))
    {
        throw db_exception("database cannot be created");
    }
}

namespace
{
int callback(void* strategyPtr, int argc, char** argv, char** azColName)
{
    if (strategyPtr)
    {

        IDatabaseEngine::ICallbackStrategy* strategy
            = static_cast<IDatabaseEngine::ICallbackStrategy*>(strategyPtr);

        strategy->NewRecord();

        for (int i = 0; i < argc; ++i)
        {
            strategy->AddColumnValue(azColName[i] ? azColName[i] : "", argv[i] ? argv[i] : "");
        }

        strategy->ApplyRecord();
    }

    return 0;
}
}

bool SqliteEngine::ExecuteQuery(const std::string& sqlquery,
                                ICallbackStrategy& callbackStrategy) const
{
    sqlite3* db;
    char* zErrMsg = 0;

    if (sqlite3_open(m_pathToDatabase.c_str(), &db) != SQLITE_OK)
    {
        sqlite3_close(db);

        return false;
    }

    if (sqlite3_exec(db, sqlquery.c_str(), callback, &callbackStrategy, &zErrMsg) != SQLITE_OK)
    {
        sqlite3_free(zErrMsg);
        sqlite3_close(db);

        return false;
    }

    sqlite3_close(db);

    return true;
}

bool SqliteEngine::ExecuteNonQuery(const std::string& sqlquery) const
{
    sqlite3* db;
    char* zErrMsg = 0;

    if (sqlite3_open(m_pathToDatabase.c_str(), &db) != SQLITE_OK)
    {
        sqlite3_close(db);

        return false;
    }

    if (sqlite3_exec(db, sqlquery.c_str(), NULL, NULL, &zErrMsg) != SQLITE_OK)
    {
        sqlite3_free(zErrMsg);
        sqlite3_close(db);

        return false;
    }

    sqlite3_close(db);

    return true;
}

}  // namespace sqlite
}  // namespace hb
