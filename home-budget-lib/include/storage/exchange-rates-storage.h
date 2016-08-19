#pragma once
#include "hb-types.h"
#include "currency_exchange/exchange-rates.h"
namespace hb
{
namespace storage
{
class ExchangeRatesStorage
{
public:
    virtual bool ContainsExchangeRates(const hb::Date& date) const = 0;

    virtual void UpdateExchangeRates(const Date& date,
                                     const hb::currency_exchange::ExchangeRatesTable& rates)
        = 0;

    virtual hb::currency_exchange::ExchangeRates GetExhangeRates(const Date& date,
                                                                 hb::CurrencyId currency) const = 0;
};

}  // namespace storage
}  // namespace hb
