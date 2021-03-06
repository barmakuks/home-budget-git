#pragma once

#include <string>
#include <map>
#include <memory>
#include <vector>

#include <hb-types.h>

namespace hb
{

class Account
{
public:
    Account();

    AccountId Id() const { return m_id; }
    void SetId(AccountId accountId) { m_id = accountId; }

    std::string Name() const { return m_name; }
    void SetName(const std::string& name) { m_name = name; }

    std::string Description() const { return m_description; }
    void SetDescription(const std::string& description) { m_description = description; }

    CurrencyId DefaultCurrency() const { return m_defaultCurrencyId; }
    void SetDefaultCurrency(CurrencyId currencyId) { m_defaultCurrencyId = currencyId; }

    int SortOrder() const { return m_sort_order; }
    void SetSortOrder(int sort_order) { m_sort_order = sort_order; }

    std::string Bank() const { return m_bank; }
    void SetBank(const std::string& bank) { m_bank = bank; }

    bool IsActive() const { return m_is_active; }
    void SetActive(bool is_active) { m_is_active = is_active; }

    Color ForegroundColor() const { return m_foreground_color; }
    void SetForegroundColor(Color color) { m_foreground_color = color; }

private:
    AccountId   m_id;
    std::string m_name;
    std::string m_description;
    CurrencyId  m_defaultCurrencyId;
    int         m_sort_order;
    std::string m_bank;
    bool        m_is_active;
    Color       m_foreground_color;
};

using  AccountPtr = std::shared_ptr<Account> ;

using AccountMap = std::map<AccountId, AccountPtr> ;
using AccountList = std::vector<AccountPtr>;
using AccountMapPtr = std::shared_ptr<AccountMap>;
using AccountListPtr = std::shared_ptr<AccountList>;

} // namespace hb
