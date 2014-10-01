#include "fill-payment-types-strategy.h"

#include <boost/lexical_cast.hpp>
#include "setters.hpp"

#include "payment-type.h"

namespace hb
{
namespace storage
{

FillPaymentTypesStrategy::FillPaymentTypesStrategy()
{
    m_values.reset(new map_type());
}

void FillPaymentTypesStrategy::NewRecord()
{
    m_value.reset(new record_type::element_type());
}

void FillPaymentTypesStrategy::ApplyRecord()
{
    if (m_value)
    {
        m_values->insert(map_type::value_type(m_value->Id(), m_value));
        m_value.reset();
    }
}

void FillPaymentTypesStrategy::AddColumnValue(const std::string& fieldName, const std::string& value)
{
    using namespace hb::core;
    std::string name = fieldName;
    std::transform(fieldName.begin(), fieldName.end(), name.begin(), ::tolower);

    if (m_value)
    {
        FieldSetter<PaymentTypeId, PaymentType, &PaymentType::SetId>::SetValue(m_value, "id", name, value);
        FieldSetter<const std::string&, PaymentType, &PaymentType::SetName>::SetValue(m_value, "name", name, value);
    }
}

} // namespace storage
} // namespace hb
