#ifndef SQLITEENGINE_H
#define SQLITEENGINE_H

#include <string>

#include "idatabase-engine.h"

namespace hb
{
namespace sqlite
{

class db_exception: public std::exception
{
public:
    db_exception(const char* what): m_what(what){}

    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT
    {
        return m_what.c_str();
    }

private:
    const std::string m_what;
};

class SqliteEngine: public hb::core::IDatabaseEngine
{
public:
    static bool IsDbExists(const std::string& path);
    static bool CreateNewDb(const std::string& path);

public:
    SqliteEngine(const std::string& path);

    virtual bool ExecuteQuery(const std::string& sqlquery, ICallbackStrategy& callbackStrategy) const;

    virtual bool ExecuteNonQuery(const std::string& sqlquery) const;

private:
    std::string m_pathToDatabase;

};

} // namespace sqlite
} // namespace hb


#endif // SQLITEENGINE_H
