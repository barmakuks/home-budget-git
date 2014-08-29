#include "gui/mainwindow.h"
#include <QApplication>

#include "database-storage.h"
#include "sqlite/sqlite-engine.h"

int main(int argc, char *argv[])
{
    using namespace hb::storage;

    hb::sqlite::SqliteEngine engine("/home/vitalii/development/barma-home-budget/data/budget.sqlite");
    DatabaseStorage storage(engine);

    QApplication a(argc, argv);
    MainWindow w(storage);
    w.show();

    return a.exec();
}
