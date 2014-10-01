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
        FieldSetter<Identifier, PaymentDocument, &PaymentDocument::SetId>::SetValue(m_value, "id", name, value);
        FieldSetter<PaymentTypeId, PaymentDocument, &PaymentDocument::SetPaymentType>::SetValue(m_value, "payment_id", name, value);
        FieldSetter<Money, PaymentDocument, &PaymentDocument::SetAmount>::SetValue(m_value, "amount", name, value);

        FieldSetter<const std::string&, PaymentDocument, &PaymentDocument::SetDocDate>::SetValue(m_value, "date", name, value);
        FieldSetter<const std::string&, PaymentDocument, &PaymentDocument::SetPeriod>::SetValue(m_value, "period", name, value);
        FieldSetter<const std::string&, PaymentDocument, &PaymentDocument::SetCounter1>::SetValue(m_value, "counter_1", name, value);
        FieldSetter<const std::string&, PaymentDocument, &PaymentDocument::SetCounter2>::SetValue(m_value, "counter_2", name, value);
        FieldSetter<const std::string&, PaymentDocument, &PaymentDocument::SetNote>::SetValue(m_value, "note", name, value);
    }
}

} // namespace storage
} // namespace hb
