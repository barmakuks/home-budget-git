#ifndef LASTIDSTRATEGY_H
#define LASTIDSTRATEGY_H

#include "idatabase-engine.h"
#include <boost/lexical_cast.hpp>
#include "raw-types.h"

namespace hb
{
namespace storage
{

class LastIdStrategy: public hb::core::IDatabaseEngine::ICallbackStrategy
{
public:
    typedef Identifier ResultType;

    LastIdStrategy(): m_lastId(EmptyId)
    {
    }

    virtual void NewRecord()
    {
        m_lastId = EmptyId;
    }

    virtual void ApplyRecord()
    {
    }

    virtual void AddColumnValue(const std::string& fieldName, const std::string& value);

    const ResultType Result() const
    {
        return m_lastId;
    }

private:
    ResultType  m_lastId;
};

} // namespace storage
} // namespace hb

#endif // LASTIDSTRATEGY_H
