#ifndef CURRENCYEXCHANGEMANAGER_H
#define CURRENCYEXCHANGEMANAGER_H

#include <memory>
#include <map>
#include <vector>

#include "currency-rates-interfaces.h"

namespace hb
{
namespace core
{

typedef std::shared_ptr<ICurrencyRatesProvider> CurrencyRatesProviderPtr;
class CurrencyRatesStoragePtr;

class CurrencyExchangeManager: public ICurrencyRatesReceiver
{
public:

    static void AddRatesProvider(const CurrencyRatesProviderPtr& ratesProvider);

    static void RequestRates(const Date& date, ICurrencyRatesReceiver* listener);

    static bool IsThisLastRequest();

private:
    static std::shared_ptr<CurrencyExchangeManager> instance;

protected:
    CurrencyExchangeManager()
    {}

    void SetRatesStorage(const CurrencyRatesStoragePtr& ratesStorage);
    void AddProvider(const CurrencyRatesProviderPtr& ratesProvider);

private:
    void SendRequest(const Date& date, ICurrencyRatesReceiver* listener);

    void AddListener(const Date& date, ICurrencyRatesReceiver* listener);
    void RemoveListeners(const Date& date);

    // Overwrite ICurrencyRatesReceiver
    virtual void OnCurrencyExchangeRatesReceived(const Date& date, const ExchangeRateTable& rates);

private:
    typedef std::vector<ICurrencyRatesReceiver*>    Listeners;
    typedef std::map<Date, Listeners>               ListenersByDate;

    ListenersByDate                                 m_listeners;

    typedef std::vector<CurrencyRatesProviderPtr>   Providers;
    Providers                                       m_providers;
    Providers::const_iterator                       m_current_provider;
};

} // namespace core
} // namespace hb

#endif // CURRENCYEXCHANGEMANAGER_H
