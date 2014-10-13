#ifndef WEBCURRENCYRATESPROVIDER_H
#define WEBCURRENCYRATESPROVIDER_H

#include "i-exchange-rates.h"
#include "iweb-engine.h"
#include "raw-types.h"

class WebCurrencyRatesProvider:
        public hb::core::ICurrencyExchangeRatesProvider,
        public hb::core::IRequestListener
{
public:
    WebCurrencyRatesProvider();

public:
    // ICurrencyExchangeRatesProvider interface
    virtual void RequestRates(const hb::Date& date,
                              hb::core::ICurrencyExchangeRatesReceiver* ratesReceiver);
    // IRequestListener interface
    virtual void OnWebResponseRecieved(hb::core::IWebEngine::RequestId requestId, const std::string& response);

    virtual hb::core::ExchangeRateTable ParseResponse(const std::string& response) = 0;
    virtual std::string GetRequestUrl() const = 0;
    virtual std::string GetRequestParameters() const = 0;

private:
    hb::core::ICurrencyExchangeRatesReceiver* m_ratesReceiver;
    hb::Date  m_date;

};


#endif // WEBCURRENCYRATESPROVIDER_H
