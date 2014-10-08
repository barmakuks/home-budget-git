#include "fill-balance-map-strategy.h"
#include "setters.hpp"

namespace hb
{
namespace storage
{

FillBalanceMapStrategy::FillBalanceMapStrategy():
    m_values(new map_type())
{
}

void FillBalanceMapStrategy::NewRecord()
{
    m_value.reset(new record_type::element_type());
}

void FillBalanceMapStrategy::ApplyRecord()
{
    if (m_value)
    {
        m_values->push_back(m_value);
        m_value.reset();
    }
}

void FillBalanceMapStrategy::AddColumnValue(const std::string& fieldName,
                                             const std::string& value)
{
    using namespace hb::core;

    std::string name = fieldName;
    std::transform(fieldName.begin(), fieldName.end(), name.begin(), ::tolower);

    if (m_value)
    {
        SetFieldValue(m_value, &BalanceRow::SetAccount, "account_id", name, value);
        SetFieldValue(m_value, &BalanceRow::SetAccount, "account_id", name, value);
        SetFieldValue(m_value, &BalanceRow::SetCurrency, "account_cur", name, value);
        SetFieldValue(m_value, &BalanceRow::SetAmount, "amount", name, value);
    }
}

} // namespace storage
} // namespace hb
