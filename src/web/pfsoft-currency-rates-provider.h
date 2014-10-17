#ifndef PFSOFTCURRENCYRATESPROVIDER_H
#define PFSOFTCURRENCYRATESPROVIDER_H

#include "web-currency-rates-provider.h"

class PfSoftCurrencyRatesProvider: public WebCurrencyRatesProvider
{
public:
    PfSoftCurrencyRatesProvider();

    // WebCurrencyRatesProvider interface
protected:
    virtual hb::core::ExchangeRateTable ParseResponse(const std::string& response);
    virtual std::string GetRequestUrl() const;
    virtual std::string GetRequestParameters(const hb::Date& date) const;
};

#endif // PFSOFTCURRENCYRATESPROVIDER_H
