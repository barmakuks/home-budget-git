#include "b911-currency-rates-provider.h"


#include "tinyxml2.h"
#include <boost/lexical_cast.hpp>

using namespace tinyxml2;
class ExchangeRateFromB911Xml: public tinyxml2::XMLVisitor
{
private:
    typedef std::map<std::string, std::string> ParseMap;
    typedef std::map<std::string, hb::CurrencyId> CurrencyMap;

public:

    ExchangeRateFromB911Xml()
    {
        if (currencyMap.empty())
        {
            currencyMap["USD"] = 840;
            currencyMap["UAH"] = 980;
            currencyMap["EUR"] = 978;
            currencyMap["RUB"] = 643;
        }
    }

    struct CurRow
    {
        std::string cur_code_lit;
        std::string cur_name;
        int amount = 0;
        double exch_rate = 0.;
    };

    typedef std::shared_ptr<CurRow> CurRowPtr;

    bool VisitEnter(const XMLElement& element, const XMLAttribute* /*attribute*/) override
    {
        if (!element.Value())
        {
            return true;
        }

        const std::string value = element.Value();

        if (value.empty())
        {
            return true;
        }

        if (value == "NBU_GRV")
        {
            if (m_current)
            {
                auto it = currencyMap.find(m_current->cur_code_lit);

                if (it != currencyMap.end() && m_current->amount)
                {
                    m_result[980][it->second] = m_current->exch_rate / m_current->amount;
                }
            }

            m_current.reset(new CurRow());
        }

        if (m_current && value == "CodeLit")
        {
            const std::string text = element.GetText();
            m_current->cur_code_lit = text;
        }

        if (m_current && value == "Amount")
        {
            const std::string text = element.GetText();
            m_current->amount = boost::lexical_cast<int>(text);
        }

        if (m_current && value == "Exch")
        {
            const std::string text = element.GetText();
            m_current->exch_rate = boost::lexical_cast<double>(text);
        }

        if (m_current && value == "Name")
        {
            const std::string text = element.GetText();
            m_current->cur_name = text;
        }

        return true;
    }

    hb::core::ExchangeRateTable Result() const
    {
        return m_result;
    }

private:
    hb::core::ExchangeRateTable m_result;
    static CurrencyMap          currencyMap;
    CurRowPtr   m_current;
};

ExchangeRateFromB911Xml::CurrencyMap ExchangeRateFromB911Xml::currencyMap;


B911CurrencyRatesProvider::B911CurrencyRatesProvider()
{

}

hb::core::ExchangeRateTable B911CurrencyRatesProvider::ParseResponse(const std::string& response)
{
    using namespace tinyxml2;
    tinyxml2::XMLDocument doc;

    if (doc.Parse(response.c_str()) == XML_SUCCESS)
    {
        ExchangeRateFromB911Xml b911Parser;
        doc.Accept(&b911Parser);

        return b911Parser.Result();
    }

    return hb::core::ExchangeRateTable();
}

std::string B911CurrencyRatesProvider::GetRequestUrl() const
{
    return "http://buhgalter911.com//Services/ExchangeRateNBU.asmx/GetRates";
}

std::string B911CurrencyRatesProvider::GetRequestParameters(const hb::Date& date) const
{
    return "date=" + date.substr(4, 2) + "/" + date.substr(6, 2) + "/" + date.substr(0, 4); // MM/DD/YYYY
}
