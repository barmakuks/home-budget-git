#pragma once
#include "hb-types.h"
namespace hb
{
namespace currency_exchange
{
class ExchangeRatesClient;

class ExchangeRatesProvider
{
public:
    virtual void RequestExchangeRates(const hb::Date& date, ExchangeRatesClient* client) = 0;
};
}  // namespace currency_exchange
}  // namespace hb
