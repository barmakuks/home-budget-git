#include "web-engine.h"
#include "qt-web-engine.h"
#include "currency-exchange-manager.h"
#include "privatbank-currency-rates-provider.h"
#include "pfsoft-currency-rates-provider.h"
#include "currency-rates-interfaces.h"
#include "convert-utils.h"

#include <iostream>
#include <QApplication>
#include <QDate>

using namespace hb::core;

namespace{
class App: public QApplication
{
public:
    App(int& argc, char* argv[]):QApplication(argc, argv){}

    bool notify(QObject* receiver, QEvent* event)
    {
        try
        {
            return QApplication::notify(receiver, event);
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
            return false;
        }
    }
};

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

        application->exit();
    }
};
}

TestReceiver test;

void RunWebServiceTest()
{
    int argc = 0; char* argv[]= {};
    application = new App(argc, argv);

    WebEngine::Setup(IWebEnginePtr(new hb::web::QtWebEngine()));

//    CurrencyExchangeManager::AddRatesProvider(CurrencyRatesProviderPtr(new PfSoftCurrencyRatesProvider()));
    CurrencyExchangeManager::AddRatesProvider(CurrencyRatesProviderPtr(new PrivatbankCurrencyRatesProvider()));

    CurrencyExchangeManager::RequestRates(hb::utils::NormalizeDate(QDate::currentDate()), &test);
    CurrencyExchangeManager::RequestRates("20140201", &test);
    CurrencyExchangeManager::RequestRates("20140301", &test);
    application->exec();

    delete application;
}
