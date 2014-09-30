#include "fill-payments-balance-strategy.h"
#include "setters.hpp"

namespace hb
{
namespace storage
{

FillPaymentsBalanceStrategy::FillPaymentsBalanceStrategy()
{
    m_values.reset(new map_type());
}

void FillPaymentsBalanceStrategy::NewRecord()
{
    m_value.reset(new record_type::element_type());
}

void FillPaymentsBalanceStrategy::ApplyRecord()
{
    if (m_value)
    {
        m_values->push_back(m_value);
        m_value.reset();
    }
}

void FillPaymentsBalanceStrategy::AddColumnValue(const std::string& fieldName, const std::string& value)
{
    using namespace hb::core;

    std::string name = fieldName;
    std::transform(fieldName.begin(), fieldName.end(), name.begin(), ::tolower);

    if (m_value)
    {
        FieldSetter<PaymentTypeId, PaymentsBalanceRow, &PaymentsBalanceRow::SetPaymentType>::SetValue(m_value, "id", name, value);
        FieldSetter<const std::string&, PaymentsBalanceRow, &PaymentsBalanceRow::SetPaymentName>::SetValue(m_value, "name", name, value);
        FieldSetter<Money, PaymentsBalanceRow, &PaymentsBalanceRow::SetAmount>::SetValue(m_value, "amount", name, value);
    }
}

} // namespace storage
} // namespace hb
