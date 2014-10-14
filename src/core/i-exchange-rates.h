#ifndef IEXCHANGERATES_H
#define IEXCHANGERATES_H

#include <map>
#include "raw-types.h"

namespace hb
{
namespace core
{
typedef std::map<CurrencyId, double> ExchangeRate;
class ExchangeRateTable: public std::map<CurrencyId, ExchangeRate>{};

class ICurrencyExchangeRatesReceiver
{
public:
    virtual void OnCurrencyExchangeRatesReceived(const Date& date, const ExchangeRateTable& rates) = 0;
};

class ICurrencyExchangeRatesProvider
{
public:
    virtual void RequestRates(const Date& date, ICurrencyExchangeRatesReceiver* ratesReceiver) = 0;
};

} // namespace core
} // namespace hb
#endif // IEXCHANGERATES_H
