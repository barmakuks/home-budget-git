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
        FieldSetter<AccountId, Account, &Account::SetId>::SetValue(m_account, "id", name, value);
        FieldSetter<const std::string&, Account, &Account::SetName>::SetValue(m_account, "name", name, value);
        FieldSetter<const std::string&, Account, &Account::SetDescription>::SetValue(m_account, "description", name, value);
        FieldSetter<CurrencyId, Account, &Account::SetDefaultCurrency>::SetValue(m_account, "default_currency_code", name, value);
        FieldSetter<int, Account, &Account::SetSortOrder>::SetValue(m_account, "sort_order", name, value);
        FieldSetter<const std::string&, Account, &Account::SetBank>::SetValue(m_account, "bank", name, value);
        FieldSetter<bool, Account, &Account::SetActive>::SetValue(m_account, "is_active", name, value);
        FieldSetter<Color, Account, &Account::SetForegroundColor>::SetValue(m_account, "foreground_color", name, value);
    }
}

} // namespace storage
} // namespace hb

