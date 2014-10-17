#include "web-engine.h"
#include "qt-web-engine.h"
#include "currency-exchange-manager.h"
#include "privatbank-currency-rates-provider.h"
#include "currency-rates-interfaces.h"

#include <iostream>
#include <QApplication>

using namespace hb::core;

QApplication* application;

class TestReceiver: public ICurrencyRatesReceiver
{
    // ICurrencyRatesReceiver interface
public:
    virtual void OnCurrencyExchangeRatesReceived(const hb::Date& date, const ExchangeRateTable& rates)
    {
        for (ExchangeRateTable::const_iterator it = rates.begin();
             it != rates.end();
             ++it)
        {
            std::cout << it->first << std::endl;

            for (ExchangeRate::const_iterator it2 = it->second.begin();
                 it2 != it->second.end();
                 ++it2)
            {
                std::cout << "\t" << it2->first << ": " << it2->second << std::endl;
            }
        }

        //application->exit();
    }
};

TestReceiver test;

void RunWebServiceTest()
{
    int argc = 0; char* argv[]= {};
    application = new QApplication(argc, argv);

    WebEngine::Setup(IWebEnginePtr(new hb::web::QtWebEngine()));

    CurrencyExchangeManager::AddRatesProvider(CurrencyRatesProviderPtr(new PrivatbankCurrencyRatesProvider()));

    CurrencyExchangeManager::RequestRates("01012014", &test);
    CurrencyExchangeManager::RequestRates("01022014", &test);
    CurrencyExchangeManager::RequestRates("01032014", &test);
    application->exec();

    delete application;
}
