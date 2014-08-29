#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui/qstandarditemmodel.h>

MainWindow::MainWindow(hb::core::IStorage& storage,
                       QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_storage(storage),
    m_balanceModel(storage, "20130101")
{
    ui->setupUi(this);

//    QStandardItemModel *model = new QStandardItemModel(2,3,this); //2 Rows and 3 Columns
//    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Column1 Header")));
//    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Column2 Header")));
//    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Column3 Header")));

    ui->tableView->setModel(&m_balanceModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

