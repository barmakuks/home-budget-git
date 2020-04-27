#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <getopt.h>

#include <database-storage.h>
#include <sqlite-engine.h>
#include "get-doc-engine.h"

class App : public QApplication
{
public:
    App(int& argc, char* argv[])
        : QApplication(argc, argv)
    {
    }

    bool notify(QObject* receiver, QEvent* event)
    {
        try
        {
            return QApplication::notify(receiver, event);
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            return false;
        }
    }
};

int main(int argc, char* argv[])
{
    using namespace hb::sql_storage;
    using namespace hb;

    App a(argc, argv);

    //    WebEngine::Setup(IWebEnginePtr(new QtWebEngine()));
    //    CurrencyExchangeManager::AddRatesProvider(CurrencyRatesProviderPtr(new
    //    PrivatbankCurrencyRatesProvider()));

    QLocale::setDefault(QLocale(QLocale::Language::English, QLocale::Country::UnitedStates));

    std::string path_to_db = "/home/vitalii/development/home-budget-git/data/budget.sqlite";

    if (argc > 1)
    {
        path_to_db = argv[1];
    }

    hb::sqlite::SqliteEngine db_engine(path_to_db);
    storage::StoragePtr storage(new DatabaseStorage(db_engine));

    InitDocEngine(storage);

    //    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    MainWindow w;
    w.show();

    return a.exec();
}
