#ifndef PRIVATBANKCURRENCYRATESPROVIDER_H
#define PRIVATBANKCURRENCYRATESPROVIDER_H

#include "web-currency-rates-provider.h"

class PrivatbankCurrencyRatesProvider: public WebCurrencyRatesProvider
{
public:
    PrivatbankCurrencyRatesProvider();

    // WebCurrencyRatesProvider interface
public:
    virtual hb::core::ExchangeRateTable ParseResponse(const std::string &response);
    virtual std::string GetRequestUrl() const;
    virtual std::string GetRequestParameters() const;
};

#endif // PRIVATBANKCURRENCYRATESPROVIDER_H
