#include "privatbank-currency-rates-provider.h"
#include "tinyxml2.h"

PrivatbankCurrencyRatesProvider::PrivatbankCurrencyRatesProvider()
{
}

using namespace tinyxml2;
class ExchangeRateFromPrivatbankXml: public tinyxml2::XMLVisitor
{
public:
    virtual bool VisitEnter( const XMLElement& element, const XMLAttribute* attribute )
    {
//        std::cout << element.Value() << std::endl;
        while ( attribute )
        {
//            std::cout << "\t" << attribute->Name() << "=" << attribute->Value();
            attribute = attribute->Next();
        }

//        std::cout << std::endl;

        return true;
    }

    hb::core::ExchangeRateTable Result() const
    {
        return m_result;
    }
private:
    hb::core::ExchangeRateTable m_result;
};

hb::core::ExchangeRateTable PrivatbankCurrencyRatesProvider::ParseResponse(const std::string& response)
{
    using namespace tinyxml2;
    tinyxml2::XMLDocument doc;
    doc.Parse(response.c_str());
    ExchangeRateFromPrivatbankXml privatParser;
    doc.Accept(&privatParser);

    return privatParser.Result();
}

std::string PrivatbankCurrencyRatesProvider::GetRequestUrl() const
{
    return "https://api.privatbank.ua/p24api/pubinfo";
}

std::string PrivatbankCurrencyRatesProvider::GetRequestParameters() const
{
    return "exchange&coursid=3";
}
