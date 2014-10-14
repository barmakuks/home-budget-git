#ifndef BALANCE_H
#define BALANCE_H

#include <vector>
#include <memory>

#include "raw-types.h"

namespace hb
{
namespace core
{

class ExchangeRateTable;

class BalanceRow
{
public:
    BalanceRow():
        m_account(EmptyId),
        m_currency(EmptyId),
        m_amount(0)
    {}

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

typedef std::shared_ptr<BalanceRow> BalanceRowPtr;


typedef std::vector<BalanceRowPtr>      TotalBalance;
typedef std::shared_ptr<TotalBalance>   TotalBalancePtr;

class Balance: public std::vector<BalanceRowPtr>
{
public:
    TotalBalancePtr GetTotalBalance() const;

    TotalBalancePtr GetTotalBalanceInCurrency(CurrencyId currencyId, hb::core::ExchangeRateTable& ratesTable) const;
};

typedef std::shared_ptr<Balance>    BalancePtr;

} // namespace core
} // namespace hb

#endif // BALANCE_H
