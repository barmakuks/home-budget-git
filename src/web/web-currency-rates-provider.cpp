#include "web-currency-rates-provider.h"
#include "web-engine.h"

WebCurrencyRatesProvider::WebCurrencyRatesProvider():
    m_ratesReceiver(NULL)
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

std::shared_ptr<hb::core::IRequestListener> requestListener;
}

void WebCurrencyRatesProvider::RequestRates(const hb::Date& date,
                                            hb::core::ICurrencyRatesReceiver* ratesReceiver)
{
    if (m_ratesReceiver == NULL && !requestListener)
    {
        m_ratesReceiver = ratesReceiver;
        m_date = date;

        requestListener.reset(new RequestListener(*this));

        hb::core::WebEngine::SendRequest(GetRequestUrl(), GetRequestParameters(), requestListener);
    }

}
void WebCurrencyRatesProvider::OnWebResponseRecieved(hb::core::IWebEngine::RequestId /*requestId*/,
                                                     const std::string& response)
{
    requestListener.reset();

    m_ratesReceiver->OnCurrencyExchangeRatesReceived(m_date, ParseResponse(response));
    m_date.clear();

    m_ratesReceiver = NULL;
}
