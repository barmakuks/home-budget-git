#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QDesktopWidget>
#include <QCalendarWidget>
#include <QTableView>
#include <QComboBox>
#include <iostream>

#include "date-time-utils.h"

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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_changeDateInterval(false)
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

    const int width = screenWidth * 3 / 4;
    const int height = screenHeight * 3 / 4;
    const int x = screenLeft + (screenWidth - width) / 2;
    const int y = screenTop + (screenHeight - height) / 2;

    resize(width, height);
    move( x, y );

    ui->balanceTableView->setModel(&m_balanceModel);
    ui->balanceTableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    ui->documentsTableView->setModel(&m_documentsModel);
    ui->documentsTableView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);

    const int m = 25;

    ui->documentsTableView->setColumnWidth(0, m * 4);
    ui->documentsTableView->setColumnWidth(1, m * 8);
    ui->documentsTableView->setColumnWidth(2, m * 8);
    ui->documentsTableView->setColumnWidth(3, m * 8);
    ui->documentsTableView->setColumnWidth(4, m * 3);
    ui->documentsTableView->setColumnWidth(5, m * 3);
    ui->documentsTableView->setColumnWidth(6, m * 4);
    ui->documentsTableView->setColumnWidth(7, m * 4);

    m_changeDateInterval = true;
    ui->startDateEdit->setDate(QDate::currentDate());
    ui->endDateEdit->setDate(QDate::currentDate());
    m_changeDateInterval = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calendarWidget_clicked(const QDate& date)
{
    m_balanceModel.Recalculate(date);
}

void MainWindow::on_periodComboBox_currentIndexChanged(int index)
{
    using namespace hb::utils;

    if (m_changeDateInterval)
    {
        return;
    }

    m_changeDateInterval = true;

    const DatePeriod::Period period = static_cast<DatePeriod::Period>(index);

    DateInterval interval = GetDateInterval(period);

    const QDate from = QDate::fromString(QString::fromUtf8(interval.from.c_str()), "yyyyMMdd");
    ui->startDateEdit->setDate(from);
    const QDate to = QDate::fromString(QString::fromUtf8(interval.to.c_str()), "yyyyMMdd");
    ui->endDateEdit->setDate(to);

    m_changeDateInterval = false;

    m_documentsModel.Reload(from, to);
}

void MainWindow::SetPeriodComboBox(const QDate& dateFrom, const QDate& dateTo)
{
    using namespace hb::utils;

    if (m_changeDateInterval)
    {
        return;
    }

    DateInterval interval(dateFrom.toString("yyyyMMdd").toUtf8().constData(),
                          dateTo.toString("yyyyMMdd").toUtf8().constData());

    const DatePeriod::Period period = GetDatePeriod(interval);

    m_changeDateInterval = true;
    ui->periodComboBox->setCurrentIndex(period);
    m_changeDateInterval = false;

    m_documentsModel.Reload(dateFrom, dateTo);
}


void MainWindow::on_startDateEdit_dateChanged(const QDate& /*date*/)
{
    SetPeriodComboBox(ui->startDateEdit->date(), ui->endDateEdit->date());
}

void MainWindow::on_endDateEdit_dateChanged(const QDate& /*date*/)
{
    SetPeriodComboBox(ui->startDateEdit->date(), ui->endDateEdit->date());
}
