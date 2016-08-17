#include "web-currency-rates-provider.h"
#include "web-engine.h"

WebCurrencyRatesProvider::WebCurrencyRatesProvider()
{
}

namespace
{
class RequestListener: public hb::core::IRequestListener
{
public:
    RequestListener(WebCurrencyRatesProvider& provider):
        m_provider(provider)
    {
    }
private:
    WebCurrencyRatesProvider& m_provider;

    // IRequestListener interface
public:
    virtual void OnWebResponseRecieved(hb::core::IWebEngine::RequestId requestId, const std::string &response)
    {
        m_provider.OnWebResponseRecieved(requestId, response);
    }
};

}

void WebCurrencyRatesProvider::RequestRates(const hb::Date& date,
                                            hb::core::ICurrencyRatesReceiver* ratesReceiver)
{
    if (!m_requestListener)
    {
        m_requestListener.reset(new RequestListener(*this));
    }

    if (m_ratesReceivers.find(date) == m_ratesReceivers.end())
    {
        m_ratesReceivers[date] = ratesReceiver;

        const hb::core::IWebEngine::RequestId requestId = hb::core::WebEngine::SendRequest(GetRequestUrl(), GetRequestParameters(date), m_requestListener);
        m_requests[requestId] = date;
    }
}

void WebCurrencyRatesProvider::OnWebResponseRecieved(hb::core::IWebEngine::RequestId requestId,
                                                     const std::string& response)
{
    if (m_requests.find(requestId) != m_requests.end())
    {
        const hb::Date date = m_requests[requestId];
        m_requests.erase(requestId);

        CurrencyRatesReceivers::const_iterator it = m_ratesReceivers.find(date);

        if (it != m_ratesReceivers.end() && it->second)
        {
            it->second->OnCurrencyExchangeRatesReceived(date, ParseResponse(response));
        }

        m_ratesReceivers.erase(date);

    }
}
