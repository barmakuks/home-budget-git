#ifndef FILLBALANCEMAPSTRATEGY_H
#define FILLBALANCEMAPSTRATEGY_H

#include "idatabase-engine.h"
#include "balance.h"

namespace hb
{
namespace storage
{

class FillBalanceMapStrategy: public hb::core::IDatabaseEngine::ICallbackStrategy
{
public:
    typedef core::BalancePtr ResultType;

    FillBalanceMapStrategy();

    virtual void NewRecord();

    virtual void ApplyRecord();

    virtual void AddColumnValue(const std::string& fieldName, const std::string& value);

    const ResultType& Result() const
    {
        return m_values;
    }

private:
    typedef ResultType::element_type    map_type;
    typedef map_type::value_type        record_type;

    ResultType  m_values;
    record_type m_value;
};

void SetValue(hb::core::BalanceRow& balance, const std::string& field, const std::string& value);

} // namespace storage
} // namespace hb


#endif // FILLBALANCEMAPSTRATEGY_H
