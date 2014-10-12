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

using namespace tinyxml2;
class ExchangeRateFromXml: public tinyxml2::XMLVisitor
{
public:
//    virtual bool VisitEnter( const XMLDocument& /*doc*/ ){}
//    virtual bool VisitExit( const XMLDocument& /*doc*/ ){}

    virtual bool VisitEnter( const XMLElement& element, const XMLAttribute* attribute )
    {
        std::cout << element.Value() << std::endl;
        while ( attribute )
        {
            std::cout << "\t" << attribute->Name() << "=" << attribute->Value();
            attribute = attribute->Next();
        }

        std::cout << std::endl;

        return true;
    }
    virtual bool VisitExit( const XMLElement& element )
    {
        return true;
    }

//    virtual bool Visit( const XMLText& text );
//    virtual bool Visit( const XMLComment& comment );
//    virtual bool Visit( const XMLDeclaration& declaration );
//    virtual bool Visit( const XMLUnknown& unknown );

};

void CurrencyExchangeRateProvider::OnWebResponseRecieved(IWebEngine::RequestId requestId, const std::string &response)
{
    using namespace tinyxml2;
    tinyxml2::XMLDocument doc;
    doc.Parse(response.c_str());

    ExchangeRateFromXml xml;
    doc.Accept(&xml);
    XMLPrinter;

//    std::cout << response << std::endl;
}

} // namespace core
} // namespace hb


