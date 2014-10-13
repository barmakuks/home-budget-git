#include "gui/mainwindow.h"
#include <QApplication>
#include <QTextCodec>

#include "database-storage.h"
#include "sqlite/sqlite-engine.h"
#include "engine.h"
#include "web-engine.h"
#include "gui/web/qt-web-engine.h"
#include "currency-exchange-manager.h"
#include "gui/web/privatbank-currency-rates-provider.h"

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

int main(int argc, char* argv[])
{
    using namespace hb::storage;
    using namespace hb::core;
    using namespace hb::web;

    App a(argc, argv);

    WebEngine::Setup(IWebEnginePtr(new QtWebEngine()));
    CurrencyExchangeManager::AddRatesProvider(ICurrencyExchangeRatesProviderPtr(new PrivatbankCurrencyRatesProvider()));
    CurrencyExchangeManager::RequestRates("", NULL);

    hb::sqlite::SqliteEngine db_engine("/home/vitalii/development/barma-home-budget/data/budget.sqlite");
    IStoragePtr storage(new DatabaseStorage(db_engine));

    Engine::CreateInstance(storage);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));


    MainWindow w;
    w.show();

    return a.exec();
}
