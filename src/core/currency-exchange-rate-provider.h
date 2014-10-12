#ifndef CURRENCYEXCHANGERATEPROVIDER_H
#define CURRENCYEXCHANGERATEPROVIDER_H

#include <string>
#include <vector>
#include <memory>
#include "iweb-engine.h"

#include "currency.h"

namespace hb
{
namespace core
{

typedef std::map<CurrencyId, double> ExchangeRate;
typedef std::map<CurrencyId, ExchangeRate> ExchangeRateTable;

class CurrencyExchangeRateProvider: public IRequestListener
{
private:
    CurrencyExchangeRateProvider();

public:

    typedef std::vector<std::string > Rates;

    static void RequestRates();

private:

    virtual void OnWebResponseRecieved(IWebEngine::RequestId requestId, const std::string &response);

private:

    ExchangeRateTable   m_exchangeTable;

private:
    static std::shared_ptr<CurrencyExchangeRateProvider> m_instance;
};

} // namespace core
} // namespace hb

#endif // CURRENCYEXCHANGERATEPROVIDER_H
