#pragma once
#pragma once

namespace hb
{
class Date;
class ExchangeRateTable;

class ICurrencyRatesClient
{
public:
    virtual void OnCurrencyExchangeRatesReceived(const Date& date, const ExchangeRateTable& rates)
        = 0;
};

}  // namespace hb
