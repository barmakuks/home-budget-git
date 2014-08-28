#ifndef BALANCE_H
#define BALANCE_H

#include <set>
#include <memory>

#include "raw-types.h"

namespace hb
{
namespace core
{

class Balance
{
public:
    Balance(){}

    Date BalanceDate() const { return m_date; }
    void SetBalanceDate(Date date) { m_date = date; }

    AccountId Account() const { return m_account; }
    void SetAccount(AccountId accountId) { m_account = accountId; }

    CurrencyId Currency() const { return m_currency; }
    void SetCurrency(CurrencyId currencyId) { m_currency = currencyId; }

    Money Amount() const { return m_amount; }
    void SetAmount(Money amount) { m_amount = amount; }

private:
    Date        m_date;
    AccountId   m_account;
    CurrencyId  m_currency;
    Money       m_amount;
};

typedef std::shared_ptr<Balance>                    BalancePtr;

class BalanceComparator
{
public:
    bool operator()(const BalancePtr& first, const BalancePtr& second)
    {
        return (first->Account() == second->Account() && first->Currency() < second->Currency())
                || (first->Account() < second->Account());
    }
};

typedef std::set<BalancePtr, BalanceComparator>     BalanceSet;
typedef std::shared_ptr<BalanceSet>                 BalanceSetPtr;

} // namespace core
} // namespace hb

#endif // BALANCE_H
