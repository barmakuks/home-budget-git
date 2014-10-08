#include "fill-accounts-map-strategy.h"
#include "setters.hpp"

namespace hb
{
namespace storage
{

FillAccountsMapStrategy::FillAccountsMapStrategy():
    m_accounts(new map_type())
{
}

void FillAccountsMapStrategy::NewRecord()
{
    m_account.reset(new account_type());
}

void FillAccountsMapStrategy::ApplyRecord()
{
    if (m_account)
    {
        m_accounts->insert(map_type::value_type(m_account->Id(), m_account));
    }
}

void FillAccountsMapStrategy::AddColumnValue(const std::string& fieldName,
                                             const std::string& value)
{
    using namespace hb::core;

    std::string name = fieldName;
    std::transform(fieldName.begin(), fieldName.end(), name.begin(), ::tolower);

    if (m_account)
    {
        SetFieldValue(m_account, &Account::SetId, "id", name, value);
        SetFieldValue(m_account, &Account::SetName, "name", name, value);
        SetFieldValue(m_account, &Account::SetDescription, "description", name, value);
        SetFieldValue(m_account, &Account::SetDefaultCurrency, "default_currency_code", name, value);
        SetFieldValue(m_account, &Account::SetSortOrder, "sort_order", name, value);
        SetFieldValue(m_account, &Account::SetBank, "bank", name, value);
        SetFieldValue(m_account, &Account::SetActive, "is_active", name, value);
        SetFieldValue(m_account, &Account::SetForegroundColor, "foreground_color", name, value);
    }
}

} // namespace storage
} // namespace hb

