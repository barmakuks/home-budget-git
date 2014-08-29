#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "core.h"
#include "models/balance-model.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(hb::core::IStorage& storage, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow*         ui;
    hb::core::IStorage&     m_storage;
    BalanceModel            m_balanceModel;

};

#endif // MAINWINDOW_H
