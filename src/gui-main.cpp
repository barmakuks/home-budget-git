#include "gui/mainwindow.h"
#include <QApplication>
#include <QTextCodec>

#include "database-storage.h"
#include "sqlite/sqlite-engine.h"
#include "model.h"

int main(int argc, char *argv[])
{
    using namespace hb::storage;
    using namespace hb::core;

    hb::sqlite::SqliteEngine engine("/home/vitalii/development/barma-home-budget/data/budget.sqlite");
    IStoragePtr storage(new DatabaseStorage(engine));

    Model::CreateInstance(storage);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
