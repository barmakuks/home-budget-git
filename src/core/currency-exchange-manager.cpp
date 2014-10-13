#include "currency-exchange-manager.h"

#include "web-engine.h"
#include <iostream>
#include <algorithm>
#include "tinyxml2.h"

namespace hb
{
namespace core
{

std::shared_ptr<CurrencyExchangeManager> CurrencyExchangeManager::m_instance;

void CurrencyExchangeManager::AddRatesProvider(const ICurrencyExchangeRatesProviderPtr& ratesProvider)
{
    // create instance if needed
    if (!m_instance)
    {
        m_instance.reset(new CurrencyExchangeManager());
    }
    // append rates provider to list
    m_instance->AddProvider(ratesProvider);
}

void CurrencyExchangeManager::RequestRates(const Date& date, ICurrencyExchangeRatesReceiver* listener)
{
    ASSERT_RETURN_VOID(m_instance);

    m_instance->Request(date, listener);
}

void CurrencyExchangeManager::AddProvider(const ICurrencyExchangeRatesProviderPtr& ratesProvider)
{
    ASSERT_RETURN_VOID((std::find(m_ratesProviders.begin(), m_ratesProviders.end(), ratesProvider) == m_ratesProviders.end()));
    m_ratesProviders.push_back(ratesProvider);
}

void CurrencyExchangeManager::Request(const Date& date, ICurrencyExchangeRatesReceiver* listener)
{
    ASSERT_RETURN_VOID(!m_ratesProviders.empty());

    if (m_listeners.find(date) == m_listeners.end())
    {
        AddListener(date, listener);
        (*m_ratesProviders.begin())->RequestRates(date, this);
    }
    else
    {
        AddListener(date, listener);
    }
}

void CurrencyExchangeManager::AddListener(const Date& date, ICurrencyExchangeRatesReceiver* listener)
{
    m_listeners[date].push_back(listener);
}

void CurrencyExchangeManager::RemoveListeners(const Date& date)
{
    m_listeners.erase(date);
}

void CurrencyExchangeManager::OnCurrencyExchangeRatesReceived(const Date& date, const ExchangeRateTable& rates)
{
    Listeners& listeners = m_listeners[date];

    for (Listeners::iterator it = listeners.begin();
         it != listeners.end();
         ++it)
    {
        if (*it)
        {
            (*it)->OnCurrencyExchangeRatesReceived(date, rates);
        }
    }

    RemoveListeners(date);
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

//template<class T>
//void CurrencyExchangeManager<T>::OnWebResponseRecieved(IWebEngine::RequestId requestId, const std::string &response)
//{
//    using namespace tinyxml2;
//    tinyxml2::XMLDocument doc;
//    doc.Parse(response.c_str());

//    ExchangeRateFromXml xml;
//    doc.Accept(&xml);

//    std::cout << response << std::endl;
//}

} // namespace core
} // namespace hb


