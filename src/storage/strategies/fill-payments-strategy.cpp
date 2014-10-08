#include "fill-payments-strategy.h"
#include "setters.hpp"

namespace hb
{
namespace storage
{

FillPaymentsStrategy::FillPaymentsStrategy()
{
    m_values.reset(new list_type());
}

void FillPaymentsStrategy::NewRecord()
{
    m_value.reset(new record_type::element_type());
}

void FillPaymentsStrategy::ApplyRecord()
{
    if (m_value)
    {
        m_values->push_back(m_value);
        m_value.reset();
    }
}

void FillPaymentsStrategy::AddColumnValue(const std::string& fieldName, const std::string& value)
{
    using namespace hb::core;

    std::string name = fieldName;
    std::transform(fieldName.begin(), fieldName.end(), name.begin(), ::tolower);

    if (m_value)
    {
        SetFieldValue(m_value, &PaymentDocument::SetId, "id", name, value);
        SetFieldValue(m_value, &PaymentDocument::SetPaymentType, "payment_id", name, value);
        SetFieldValue(m_value, &PaymentDocument::SetAmount, "amount", name, value);

        SetFieldValue(m_value, &PaymentDocument::SetDocDate, "date", name, value);
        SetFieldValue(m_value, &PaymentDocument::SetPeriod, "period", name, value);
        SetFieldValue(m_value, &PaymentDocument::SetCounter1, "counter_1", name, value);
        SetFieldValue(m_value, &PaymentDocument::SetCounter2, "counter_2", name, value);
        SetFieldValue(m_value, &PaymentDocument::SetNote, "note", name, value);
    }
}

} // namespace storage
} // namespace hb
