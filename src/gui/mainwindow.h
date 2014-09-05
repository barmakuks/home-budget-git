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
    void SetPeriodComboBox(const QDate& dateFrom, const QDate& dateTo);

private slots:
    void on_calendarWidget_clicked(const QDate& date);

    void on_periodComboBox_currentIndexChanged(int index);

    void on_startDateEdit_dateChanged(const QDate& date);

    void on_endDateEdit_dateChanged(const QDate &date);

private:
    Ui::MainWindow*         ui;
    hb::core::IStorage&     m_storage;
    BalanceModel            m_balanceModel;

    bool                    m_changeDateInterval;

};

#endif // MAINWINDOW_H
