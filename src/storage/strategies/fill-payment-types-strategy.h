#ifndef FILLPAYMENTTYPESSTRATEGY_H
#define FILLPAYMENTTYPESSTRATEGY_H

#include "idatabase-engine.h"
#include "payment-type.h"

namespace hb
{
namespace storage
{

class FillPaymentTypesStrategy: public hb::core::IDatabaseEngine::ICallbackStrategy
{
public:
    typedef core::PaymentTypesMapPtr   ResultType;

    FillPaymentTypesStrategy();

    virtual void NewRecord();

    virtual void ApplyRecord();

    virtual void AddColumnValue(const std::string& fieldName, const std::string& value);

    const ResultType& Result() const
    {
        return m_values;
    }

private:
    typedef ResultType::element_type    map_type;
    typedef map_type::mapped_type       record_type;

    ResultType  m_values;
    record_type m_value;
};

} // namespace storage
} // namespace hb

#endif // FILLPAYMENTTYPESSTRATEGY_H
