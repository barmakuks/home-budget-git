#ifndef CURRENCYEXCHANGEMANAGER_H
#define CURRENCYEXCHANGEMANAGER_H

#include <memory>
#include <map>
#include <vector>

#include "i-exchange-rates.h"

namespace hb
{
namespace core
{

typedef std::shared_ptr<ICurrencyExchangeRatesProvider> ICurrencyExchangeRatesProviderPtr;

class CurrencyExchangeManager: public ICurrencyExchangeRatesReceiver
{
public:

    static void AddRatesProvider(const ICurrencyExchangeRatesProviderPtr& ratesProvider);

    static void RequestRates(const Date& date, ICurrencyExchangeRatesReceiver* listener);

private:
    static std::shared_ptr<CurrencyExchangeManager> m_instance;

private:
    CurrencyExchangeManager()
    {}

    void AddProvider(const ICurrencyExchangeRatesProviderPtr& ratesProvider);

    void Request(const Date& date, ICurrencyExchangeRatesReceiver* listener);

    void AddListener(const Date& date, ICurrencyExchangeRatesReceiver* listener);

    void RemoveListeners(const Date& date);

    virtual void OnCurrencyExchangeRatesReceived(const Date& date, const ExchangeRateTable& rates);

private:
    typedef std::vector<ICurrencyExchangeRatesReceiver*>  Listeners;
    typedef std::map<Date, Listeners>                     ListenersByDate;

    ListenersByDate                                 m_listeners;
    std::vector<ICurrencyExchangeRatesProviderPtr>  m_ratesProviders;
};

} // namespace core
} // namespace hb

#endif // CURRENCYEXCHANGEMANAGER_H
