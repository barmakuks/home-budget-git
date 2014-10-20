#ifndef WEBCURRENCYRATESPROVIDER_H
#define WEBCURRENCYRATESPROVIDER_H

#include "currency-rates-interfaces.h"
#include "iweb-engine.h"
#include "raw-types.h"
#include <map>

class WebCurrencyRatesProvider:
        public hb::core::ICurrencyRatesProvider,
        public hb::core::IRequestListener
{
public:
    WebCurrencyRatesProvider();

public:
    // ICurrencyExchangeRatesProvider interface
    virtual void RequestRates(const hb::Date& date,
                              hb::core::ICurrencyRatesReceiver* ratesReceiver);
    // IRequestListener interface
    virtual void OnWebResponseRecieved(hb::core::IWebEngine::RequestId requestId, const std::string& response);

protected:

    virtual hb::core::ExchangeRateTable ParseResponse(const std::string& response) = 0;
    virtual std::string GetRequestUrl() const = 0;
    virtual std::string GetRequestParameters(const hb::Date& date) const = 0;

private:
    typedef std::map<hb::Date, hb::core::ICurrencyRatesReceiver*>   CurrencyRatesReceivers;
    typedef std::map<hb::core::IWebEngine::RequestId, hb::Date>     Requests;

    CurrencyRatesReceivers  m_ratesReceivers;
    Requests                m_requests;
    std::shared_ptr<IRequestListener> m_requestListener;

};


#endif // WEBCURRENCYRATESPROVIDER_H
