#include "pfsoft-currency-rates-provider.h"

#include "tinyxml2.h"
#include <boost/lexical_cast.hpp>

PfSoftCurrencyRatesProvider::PfSoftCurrencyRatesProvider()
{
}

namespace
{

const hb::CurrencyId UAH = 980;

using namespace tinyxml2;
class ExchangeRateFromPfSoftXml: public XMLVisitor
{
public:

    ExchangeRateFromPfSoftXml():
        m_currentCurrencyId(hb::EmptyId)
    {
    }

    virtual bool VisitEnter(const XMLElement& element, const XMLAttribute* /*attribute*/)
    {
        if (!element.Value() || !element.GetText())
        {
            return true;
        }

        const std::string value = element.Value();
        const std::string text = element.GetText();

        if (value.empty() || text.empty())
        {
            return true;
        }

        if (value == "Valute")
        {
            m_currentCurrencyId = hb::EmptyId;
            m_nominal = 1;
        }

        if (value == "NumCode")
        {
            m_currentCurrencyId = boost::lexical_cast<hb::CurrencyId>(text);
        }

        if (value == "Nominal")
        {
            m_nominal = boost::lexical_cast<int>(text);
        }

        if (value == "Value")
        {
            const double buyRate = boost::lexical_cast<double>(text) / m_nominal;
            const double saleRate = buyRate;

            if (m_currentCurrencyId != hb::EmptyId && buyRate > 0.0 && saleRate > 0.0)
            {
                m_result[m_currentCurrencyId][UAH] = buyRate;
                m_result[UAH][m_currentCurrencyId] = 1.0 / saleRate;
            }
        }

        return true;
    }

    hb::core::ExchangeRateTable Result() const
    {
        return m_result;
    }

private:
    hb::CurrencyId  m_currentCurrencyId;
    int             m_nominal;

    hb::core::ExchangeRateTable m_result;
};

}

hb::core::ExchangeRateTable PfSoftCurrencyRatesProvider::ParseResponse(const std::string& response)
{
    using namespace tinyxml2;
    tinyxml2::XMLDocument doc;
    doc.Parse(response.c_str());

    ExchangeRateFromPfSoftXml pfSoftParser;
    doc.Accept(&pfSoftParser);

    return pfSoftParser.Result();
}

std::string PfSoftCurrencyRatesProvider::GetRequestUrl() const
{
    return "http://pfsoft.com.ua/service/currency/";

}

std::string PfSoftCurrencyRatesProvider::GetRequestParameters(const hb::Date& date) const
{
    if (date.length() != 8)
    {
        return "";
    }
    return "date=" + date.substr(6,2) + date.substr(4,2) + date.substr(0,4);
}
