#include "gui/mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <iostream>

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

    std::string path_to_db = "/home/vitalii/development/home-budget-git/data/budget.sqlite";

    if (argc > 1)
    {
        path_to_db = argv[1];
    }

    hb::sqlite::SqliteEngine db_engine(path_to_db);
    IStoragePtr storage(new DatabaseStorage(db_engine));

    Engine::CreateInstance(storage);

//    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));


    MainWindow w;
    w.show();

    return a.exec();
}
