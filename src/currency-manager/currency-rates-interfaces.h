#ifndef ICURRENCYRATESINTERFACES_H
#define ICURRENCYRATESINTERFACES_H

#include <map>
#include "raw-types.h"

namespace hb
{
namespace core
{
typedef std::map<CurrencyId, double> ExchangeRate;
class ExchangeRateTable: public std::map<CurrencyId, ExchangeRate>{};

class ICurrencyRatesReceiver
{
public:
    virtual void OnCurrencyExchangeRatesReceived(const Date& date, const ExchangeRateTable& rates) = 0;
};

class ICurrencyRatesProvider
{
public:
    virtual void RequestRates(const Date& date, ICurrencyRatesReceiver* ratesReceiver) = 0;
};

class ICurrencyRatesStorage
{
public:
    virtual void WriteRates(const Date& date, const ExchangeRateTable& rates) = 0;
};

} // namespace core
} // namespace hb
#endif // ICURRENCYRATESINTERFACES_H
