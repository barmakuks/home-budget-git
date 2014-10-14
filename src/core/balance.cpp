#include "balance.h"

#include <map>

#include "i-exchange-rates.h"

namespace hb
{
namespace core
{

TotalBalancePtr Balance::GetTotalBalance() const
{
    typedef std::map<CurrencyId, BalanceRowPtr> BalanceMap;

    BalanceMap total_balance;

    for (auto it = begin(); it != end(); ++it)
    {
        BalanceMap::iterator current = total_balance.find((*it)->Currency());

        if (current == total_balance.end())
        {
            BalanceRowPtr row(new BalanceRow());
            row->SetCurrency((*it)->Currency());
            current = total_balance.insert(BalanceMap::value_type((*it)->Currency(), row)).first;
        }

        current->second->SetAmount(current->second->Amount() + (*it)->Amount());
    }

    TotalBalancePtr balance(new TotalBalance());

    balance->reserve(total_balance.size());

    for (BalanceMap::const_iterator it = total_balance.begin();
         it != total_balance.end();
         ++it)
    {
        balance->push_back(it->second);
    }

    return balance;
}

TotalBalancePtr Balance::GetTotalBalanceInCurrency(CurrencyId currencyId, hb::core::ExchangeRateTable& ratesTable) const
{
    return TotalBalancePtr();
}

} // namespace core
} // namespace hb
