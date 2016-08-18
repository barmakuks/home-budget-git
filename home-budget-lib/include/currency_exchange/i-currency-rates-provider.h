#pragma once
namespace hb
{
class Date;
class ICurrencyRatesClient;

class ICurrencyRatesProvider
{
public:
    virtual void RequestRates(const Date& date, ICurrencyRatesClient* ratesReceiver) = 0;
};
} // namespace hb
