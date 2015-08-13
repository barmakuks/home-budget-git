#ifndef B911CURRENCYRATESPROVIDER_H
#define B911CURRENCYRATESPROVIDER_H

#include "web-currency-rates-provider.h"

class B911CurrencyRatesProvider: public WebCurrencyRatesProvider
{
public:
    B911CurrencyRatesProvider();

    // WebCurrencyRatesProvider interface
protected:
    virtual hb::core::ExchangeRateTable ParseResponse(const std::string& response);
    virtual std::string GetRequestUrl() const;
    virtual std::string GetRequestParameters(const hb::Date& date) const;
};

#endif // B911CURRENCYRATESPROVIDER_H
