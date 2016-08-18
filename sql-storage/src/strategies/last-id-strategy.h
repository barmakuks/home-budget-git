#pragma once

#include "idatabase-engine.h"
#include <boost/lexical_cast.hpp>
#include "hb-types.h"

namespace hb
{
namespace sql_storage
{
class LastIdStrategy : public IDatabaseEngine::ICallbackStrategy
{
public:
    typedef Identifier ResultType;

    LastIdStrategy()
        : m_lastId(EmptyId)
    {
    }

    void NewRecord() override
    {
        m_lastId = EmptyId;
    }

    void ApplyRecord() override
    {
    }

    void AddColumnValue(const std::string& fieldName, const std::string& value) override;

    void Finalize() override
    {
    }

    const ResultType Result() const
    {
        return m_lastId;
    }

private:
    ResultType m_lastId;
};

}  // namespace sql_storage
}  // namespace hb
