#include "privatbank-currency-rates-provider.h"
#include "tinyxml2.h"

#include <boost/lexical_cast.hpp>
#include <QDate>
#include "convert-utils.h"

PrivatbankCurrencyRatesProvider::PrivatbankCurrencyRatesProvider()
{
}

using namespace tinyxml2;
class ExchangeRateFromPrivatbankXml: public tinyxml2::XMLVisitor
{
private:
    typedef std::map<std::string, std::string> ParseMap;
    typedef std::map<std::string, hb::CurrencyId> CurrencyMap;

public:

    ExchangeRateFromPrivatbankXml()
    {
        if (currencyMap.empty())
        {
            currencyMap["USD"] = 840;
            currencyMap["UAH"] = 980;
            currencyMap["EUR"] = 978;
            currencyMap["RUR"] = 643;
        }
    }

    virtual bool VisitEnter(const XMLElement& element, const XMLAttribute* attribute)
    {
        if (strcmp(element.Value(), "exchangerate") == 0)
        {
            ParseMap attrMap;

            while (attribute)
            {
                attrMap[attribute->Name()] = attribute->Value();
                attribute = attribute->Next();
            }

            const hb::CurrencyId baseCurId = GetCurrencyId(attrMap["base_ccy"]);
            const hb::CurrencyId curId = GetCurrencyId(attrMap["ccy"]);
            const double buyRate = GetRateValue(attrMap["buy"]);
            const double saleRate = GetRateValue(attrMap["sale"]);

            if (baseCurId != hb::EmptyId && curId != hb::EmptyId && buyRate > 0.0 && saleRate > 0.0)
            {
                m_result[baseCurId][curId] = buyRate;
                m_result[curId][baseCurId] = 1.0 / saleRate;
            }
        }

        return true;
    }

    hb::core::ExchangeRateTable Result() const
    {
        return m_result;
    }

private:
    static hb::CurrencyId GetCurrencyId(const std::string& curName)
    {
        if (!curName.empty() && currencyMap.find(curName) != currencyMap.end())
        {
            return currencyMap[curName];
        }

        return hb::EmptyId;
    }

    static double GetRateValue(const std::string& rate)
    {
        try
        {
            return boost::lexical_cast<double>(rate);
        }
        catch (boost::bad_lexical_cast const&)
        {
        }

        return -1.0;
    }

private:
    hb::core::ExchangeRateTable m_result;
    static CurrencyMap          currencyMap;
};

ExchangeRateFromPrivatbankXml::CurrencyMap ExchangeRateFromPrivatbankXml::currencyMap;


hb::core::ExchangeRateTable PrivatbankCurrencyRatesProvider::ParseResponse(const std::string& response)
{
    using namespace tinyxml2;
    tinyxml2::XMLDocument doc;

    if (doc.Parse(response.c_str()) == XML_SUCCESS)
    {
        ExchangeRateFromPrivatbankXml privatParser;
        doc.Accept(&privatParser);

        return privatParser.Result();
    }
    else
    {
        return hb::core::ExchangeRateTable();
    }
}

std::string PrivatbankCurrencyRatesProvider::GetRequestUrl() const
{
    return "https://api.privatbank.ua/p24api/pubinfo";
}

std::string PrivatbankCurrencyRatesProvider::GetRequestParameters(const hb::Date& date) const
{
    if (date == hb::utils::NormalizeDate(QDate::currentDate()))
    {
        return "exchange&coursid=3";
    }
    else
    {
        return "";
    }
}
