#include "currency-exchange-rate-provider.h"

#include "web-engine.h"
#include <iostream>
#include <stdio.h>
#include "tinyxml2.h"

namespace hb
{
namespace core
{

std::shared_ptr<CurrencyExchangeRateProvider> CurrencyExchangeRateProvider::m_instance;

CurrencyExchangeRateProvider::CurrencyExchangeRateProvider()
{
}

void CurrencyExchangeRateProvider::RequestRates()
{
    if (!m_instance)
    {
        m_instance.reset(new CurrencyExchangeRateProvider());
    }

    WebEngine::SendRequest("https://api.privatbank.ua/p24api/pubinfo?", "exchange&coursid=3", m_instance);
}

void CurrencyExchangeRateProvider::OnWebResponseRecieved(IWebEngine::RequestId requestId, const std::string &response)
{
    tinyxml2::XMLDocument doc;
    doc.Parse(response.c_str());

    FILE* file = fopen("hello-this", "w");
    tinyxml2::XMLPrinter printer(file);
    doc.Print(&printer);

    fclose(file);

//    std::cout << response << std::endl;
}

} // namespace core
} // namespace hb


