#ifndef FILLPAYMENTSBALANCESTRATEGY_H
#define FILLPAYMENTSBALANCESTRATEGY_H

#include "idatabase-engine.h"
#include "payments-balance.h"

namespace hb
{
namespace storage
{

class FillPaymentsBalanceStrategy: public hb::core::IDatabaseEngine::ICallbackStrategy
{
public:
    typedef core::PaymentsBalancePtr   ResultType;

    FillPaymentsBalanceStrategy();

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

} // namespace storage
} // namespace hb

#endif // FILLPAYMENTSBALANCESTRATEGY_H
