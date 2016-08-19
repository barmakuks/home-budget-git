#pragma once
#include "hb-types.h"
#include "exchange-rates.h"

namespace hb
{
namespace currency_exchange
{

class ExchangeRatesClient
{
public:
    virtual void OnExchangeRatesReceived(const Date& date, const ExchangeRatesTable& rates)
        = 0;
};
}  // namespace currency_exchange
}  // namespace hb
