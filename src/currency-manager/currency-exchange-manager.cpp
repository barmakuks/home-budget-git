#include "currency-exchange-manager.h"

#include "web-engine.h"
#include <iostream>
#include <algorithm>
#include "tinyxml2.h"

namespace hb
{
namespace core
{

class CurrencyRatesStoragePtr: public std::shared_ptr<ICurrencyRatesStorage>{};

/******************************************************* STATIC **********************************************************/
std::shared_ptr<CurrencyExchangeManager> CurrencyExchangeManager::instance;

void CurrencyExchangeManager::AddRatesProvider(const CurrencyRatesProviderPtr& ratesProvider)
{
    // create instance if needed
    if (!instance)
    {
        instance.reset(new CurrencyExchangeManager());
    }
    // append rates provider to list
    instance->AddProvider(ratesProvider);
}

void CurrencyExchangeManager::RequestRates(const Date& date, ICurrencyRatesReceiver* listener)
{
    ASSERT_RETURN_VOID(instance);

    instance->SendRequest(date, listener);
}

/**************************************************************************************************************************/

void CurrencyExchangeManager::AddProvider(const CurrencyRatesProviderPtr& ratesProvider)
{
    ASSERT_RETURN_VOID((std::find(m_providers.begin(), m_providers.end(), ratesProvider) == m_providers.end()));
    m_providers.push_back(ratesProvider);
}

void CurrencyExchangeManager::SendRequest(const Date& date, ICurrencyRatesReceiver* listener)
{
    // TODO start sending request from first element in rates_provider
    ASSERT_RETURN_VOID(!m_providers.empty());

    if (m_listeners.find(date) == m_listeners.end())
    {
        AddListener(date, listener);
        (*m_providers.begin())->RequestRates(date, this);
    }
    else
    {
        AddListener(date, listener);
    }
}

void CurrencyExchangeManager::AddListener(const Date& date, ICurrencyRatesReceiver* listener)
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
    m_current_provider = m_providers.begin();
}

} // namespace core
} // namespace hb


