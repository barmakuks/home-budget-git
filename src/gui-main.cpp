#include "gui/mainwindow.h"
#include <QApplication>
#include <QTextCodec>

#include "database-storage.h"
#include "sqlite/sqlite-engine.h"
#include "engine.h"

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

    App a(argc, argv);

//    WebEngine::Setup(IWebEnginePtr(new QtWebEngine()));
//    CurrencyExchangeManager::AddRatesProvider(CurrencyRatesProviderPtr(new PrivatbankCurrencyRatesProvider()));

    hb::sqlite::SqliteEngine db_engine("/home/vitalii/home-budget/data/budget.sqlite");
    IStoragePtr storage(new DatabaseStorage(db_engine));

    Engine::CreateInstance(storage);

//    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));


    MainWindow w;
    w.show();

    return a.exec();
}
