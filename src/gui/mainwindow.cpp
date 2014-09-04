#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QDesktopWidget>
#include <QCalendarWidget>
#include <QTableView>
#include <iostream>

namespace
{
int FindBiggestScreen(const QDesktopWidget& desktop)
{
    int index = 0;
    int max_square = 0;
    for (int i = 0; i < desktop.screenCount(); ++i)
    {
        int square = desktop.screenGeometry(i).width() * desktop.screenGeometry().height();

        if (square > max_square)
        {
            max_square = square;
            index = i;
        }
    }

    return  index;
}
}

MainWindow::MainWindow(hb::core::IStorage& storage,
                       QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_storage(storage),
    m_balanceModel(storage)
{
    ui->setupUi(this);
    const QDesktopWidget& desktop = *QApplication::desktop();

    int screenWidth = desktop.width();
    int screenHeight = desktop.height();
    int screenLeft = 0;
    int screenTop = 0;

    if (desktop.screenCount() > 1)
    {
        int screenNo = FindBiggestScreen(desktop);

        screenLeft = desktop.screenGeometry(screenNo).left();
        screenTop = desktop.screenGeometry(screenNo).top();
        screenWidth = desktop.screenGeometry(screenNo).width();
        screenHeight = desktop.screenGeometry(screenNo).height();
    }

    const int x = screenLeft + (screenWidth - size().width()) / 2;
    const int y = screenTop + (screenHeight - size().height()) / 2;

    move( x, y );

    ui->tableView->setModel(&m_balanceModel);
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calendarWidget_clicked(const QDate& date)
{
    m_balanceModel.Recalculate(date);
}
