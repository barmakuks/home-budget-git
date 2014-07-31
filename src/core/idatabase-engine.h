#ifndef IDATABASEENGINE_H
#define IDATABASEENGINE_H

#include <string>

namespace hb
{
namespace core
{
class IDatabaseEngine
{
public:
    virtual ~IDatabaseEngine(){}

    class ICallbackStrategy
    {
    public:
        virtual ~ICallbackStrategy(){}
        virtual void NewRecord() = 0;
        virtual void ApplyRecord() = 0;
        virtual void AddColumnValue(const std::string& fieldName, const std::string& value) = 0;
    };

public:
    virtual bool ExecuteSql(const std::string& sqlquery, ICallbackStrategy& callbackStrategy) const = 0;
};

} // namespace core
} // namespace hg


#endif // IDATABASEENGINE_H
