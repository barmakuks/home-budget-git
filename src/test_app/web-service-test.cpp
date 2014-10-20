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

namespace
{
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
public:
    TestReceiver():
        m_responces_left(0)
    {}
    // ICurrencyRatesReceiver interface
public:
    virtual void OnCurrencyExchangeRatesReceived(const hb::Date& date, const ExchangeRateTable& rates)
    {
        std::cout << "********************** " << "Currency rates on " << date << " **********************" << std::endl;
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

        if (--m_responces_left <= 0)
        {
            application->exit();
        }
    }

    ICurrencyRatesReceiver* get()
    {
        m_responces_left++;
        return this;
    }
private:
    // disable get address of object
    TestReceiver* operator&()
    {
        std::cout << "Operator &" << std::endl;
        return this;
    }

private:
    int m_responces_left;
};
}

TestReceiver test;

void RunWebServiceTest()
{
    int argc = 0; char* argv[]= {};
    application = new App(argc, argv);

    WebEngine::Setup(IWebEnginePtr(new hb::web::QtWebEngine()));

    CurrencyExchangeManager::AddRatesProvider(CurrencyRatesProviderPtr(new PrivatbankCurrencyRatesProvider()));
    CurrencyExchangeManager::AddRatesProvider(CurrencyRatesProviderPtr(new PfSoftCurrencyRatesProvider()));

    CurrencyExchangeManager::RequestRates(hb::utils::NormalizeDate(QDate::currentDate()), test.get());
    CurrencyExchangeManager::RequestRates("20140201", test.get());
    CurrencyExchangeManager::RequestRates("20140301", test.get());
    application->exec();

    delete application;
}
