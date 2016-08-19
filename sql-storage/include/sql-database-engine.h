#pragma once

#include <string>

namespace hb
{
namespace sql_storage
{
class SqlDatabaseEngine
{
public:
    virtual ~SqlDatabaseEngine()
    {
    }

    class CallbackStrategy
    {
    public:
        virtual ~CallbackStrategy()
        {
        }
        virtual void NewRecord() = 0;
        virtual void ApplyRecord() = 0;
        virtual void AddColumnValue(const std::string& fieldName, const std::string& value) = 0;
        virtual void Finalize() = 0;
    };

public:
    virtual bool ExecuteQuery(const std::string& sqlquery,
                              CallbackStrategy& callbackStrategy) const = 0;

    virtual bool ExecuteNonQuery(const std::string& sqlquery) const = 0;
};
}  // namespace sql_storage
}  // namespace hg
