#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QDesktopWidget>
#include <QCalendarWidget>
#include <QTableView>
#include <QComboBox>
#include <iostream>
#include <QMessageBox>

#include "date-time-utils.h"
#include "document-dialog.h"
#include "movement-dialog.h"
#include "document.h"
#include "engine.h"
#include "convert-utils.h"
#include "currency-exchange-manager.h"
#include "models/string-format.h"

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

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_filterSetupInProgress(true),
    m_doc_dlg(NULL)
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

    const int width = screenWidth * 4 / 5;
    const int height = screenHeight * 3 / 4;
    const int x = screenLeft + (screenWidth - width) / 2;
    const int y = screenTop + (screenHeight - height) / 2;

    resize(width, height);
    move(x, y);

    ui->balanceTableView->setModel(&m_balanceModel);
    ui->balanceTableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    ui->paymentsBalanceTableView->setModel(&m_paymentsBalanceModel);
//    ui->paymentsTableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->paymentsTableView->setModel(&m_paymentsModel);

    ui->documentsTableView->setModel(&m_documentsModel);
    ui->documentsTableView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);

    const int m = 25;

    ui->documentsTableView->setColumnWidth(0, m * 3.5);
    ui->documentsTableView->setColumnWidth(1, m * 8);
    ui->documentsTableView->setColumnWidth(2, m * 4);
    ui->documentsTableView->setColumnWidth(3, m * 4);
    ui->documentsTableView->setColumnWidth(4, m * 8);
    ui->documentsTableView->setColumnWidth(5, m * 8);
    ui->documentsTableView->setColumnWidth(6, m * 4);
    ui->documentsTableView->setColumnWidth(7, m * 4);

    ui->accountComboBox->setModel(&m_accountsModel);

    ui->currencyComboBox->setModel(&m_currenciesModel);

    ui->startDateEdit->setDate(QDate::currentDate());
    ui->endDateEdit->setDate(QDate::currentDate());

    m_filterSetupInProgress = false;

    ApplyDocumentsFilter();

    connect(ui->documentsTableView->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
            this,
            SLOT(documentsTableView_selectionChanged(const QItemSelection&, const QItemSelection&)));
}

MainWindow::~MainWindow()
{
    if (m_doc_dlg)
    {
        delete m_doc_dlg;
        m_doc_dlg = NULL;
    }

    delete ui;
}

void MainWindow::on_calendarWidget_clicked(const QDate& /*date*/)
{
    UpdateBalance();
}

void MainWindow::on_periodComboBox_currentIndexChanged(int index)
{
    using namespace hb::utils;

    if (m_filterSetupInProgress)
    {
        return;
    }

    m_filterSetupInProgress = true;

    const DatePeriod::Period period = static_cast<DatePeriod::Period>(index);

    DateInterval interval = GetDateInterval(period);

    const QDate from = hb::utils::QDatefromNormalizedDate(interval.from);
    ui->startDateEdit->setDate(from);
    const QDate to = hb::utils::QDatefromNormalizedDate(interval.to);
    ui->endDateEdit->setDate(to);

    m_filterSetupInProgress = false;

    ApplyDocumentsFilter();
}

void MainWindow::SetPeriodComboBox(const QDate& dateFrom, const QDate& dateTo)
{
    using namespace hb::utils;

    if (m_filterSetupInProgress)
    {
        return;
    }

    DateInterval interval(hb::utils::NormalizeDate(dateFrom),
                          hb::utils::NormalizeDate(dateTo));

    const DatePeriod::Period period = GetDatePeriod(interval);

    m_filterSetupInProgress = true;
    ui->periodComboBox->setCurrentIndex(period);
    m_filterSetupInProgress = false;

    ApplyDocumentsFilter();
}

void MainWindow::ApplyDocumentsFilter()
{
    hb::AccountId accountId = m_accountsModel.GetAccountItemId(ui->accountComboBox->currentIndex());
    hb::CurrencyId currencyId = m_currenciesModel.GetCurrencyItemId(ui->currencyComboBox->currentIndex());

    QModelIndexList selection = ui->documentsTableView->selectionModel()->selectedRows();

    m_documentsModel.Reload(ui->startDateEdit->date(),
                            ui->endDateEdit->date(),
                            accountId,
                            currencyId);

    m_paymentsModel.Reload(ui->startDateEdit->date(),
                           ui->endDateEdit->date());

    if (!selection.empty())
    {
        ui->documentsTableView->selectRow(selection.begin()->row());
    }

    SetButtonsEnabled();
}

class CurEx: public hb::core::ICurrencyRatesReceiver
{
    // ICurrencyExchangeRatesReceiver interface
public:
    virtual void OnCurrencyExchangeRatesReceived(const hb::Date& date, const hb::core::ExchangeRateTable& rates)
    {
        using namespace hb::core;
        BalancePtr balance = Engine::GetInstance().GetBalance(date);
        TotalBalancePtr total_balance = balance->GetTotalBalance();

        for (Balance::const_iterator it = total_balance->begin();
             it != total_balance->end();
             ++it)
        {
            std::cout << (*it)->Currency() << " : " << hb::utils::FormatMoney((*it)->Amount());
            if ((*it)->Currency() != 980)
            {
                std::cout << " in UAH: " << hb::utils::FormatMoney(rates.at(980).at((*it)->Currency()) * (*it)->Amount()) << "uah";
            }
            std::cout << std::endl;
        }
    }
} currency;

void MainWindow::UpdateBalance()
{
    m_balanceModel.Recalculate(ui->calendarWidget->selectedDate());
    m_paymentsBalanceModel.Recalculate(ui->calendarWidget->selectedDate());

//    hb::core::CurrencyExchangeManager::RequestRates(hb::utils::NormalizeDate(ui->calendarWidget->selectedDate()), &currency);
}

void MainWindow::EditDocument()
{
    using namespace hb::core;

    QModelIndexList indexes = ui->documentsTableView->selectionModel()->selectedRows();

    if (indexes.size() == 1)
    {
        DocumentPtr doc = m_documentsModel.GetDocumentItemPtr(indexes[0].row());

        DocumentTypePtr docType = Engine::GetInstance().GetTypeList()->at(doc->DocType());

        const bool result = docType->Sign() == DocumentType::Movement ? MovementDialog::EditDocument(doc) : DocumentDialog::EditDocument(doc);

        if (result)
        {
            hb::core::Engine::GetInstance().Write(*doc);
            ApplyDocumentsFilter();
            UpdateBalance();
        }
    }
}

void MainWindow::CreateDocument(hb::core::DocumentType::TypeSign docType)
{
    using namespace hb::core;

    bool result = false;
    if (docType == hb::core::DocumentType::Movement)
    {
        result = MovementDialog::CreateDocument(docType);
    }
    else
    {
        result = DocumentDialog::CreateDocument(docType);
    }

    if (result)
    {
        ApplyDocumentsFilter();
        UpdateBalance();
    }
}

void MainWindow::SetButtonsEnabled()
{
    const bool enabled = ui->documentsTableView->selectionModel()->selectedRows().size() == 1;
    ui->editButton->setEnabled(enabled);
    ui->removeButton->setEnabled(enabled);
}

void MainWindow::on_startDateEdit_dateChanged(const QDate& /*date*/)
{
    SetPeriodComboBox(ui->startDateEdit->date(), ui->endDateEdit->date());
}

void MainWindow::on_endDateEdit_dateChanged(const QDate& /*date*/)
{
    SetPeriodComboBox(ui->startDateEdit->date(), ui->endDateEdit->date());
}

void MainWindow::on_accountComboBox_currentIndexChanged(int /*index*/)
{
    if (!m_filterSetupInProgress)
    {
        ApplyDocumentsFilter();
    }
}

void MainWindow::on_currencyComboBox_currentIndexChanged(int index)
{
    if (!m_filterSetupInProgress)
    {
        ApplyDocumentsFilter();
    }
}

void MainWindow::on_creditButton_clicked()
{
    CreateDocument(hb::core::DocumentType::Income);
}

void MainWindow::on_debitButton_clicked()
{
    CreateDocument(hb::core::DocumentType::Outcome);
}

void MainWindow::on_editButton_clicked()
{
    EditDocument();
}

void MainWindow::on_documentsTableView_doubleClicked(const QModelIndex& index)
{
    if (index.isValid())
    {
        EditDocument();
    }
}

void MainWindow::on_removeButton_clicked()
{
    QModelIndexList indexes = ui->documentsTableView->selectionModel()->selectedRows();

    if (indexes.size() == 1)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(hb::utils::Tr("Видалення документу"));
        msgBox.setText(hb::utils::Tr("Видалення документу"));
        msgBox.setInformativeText(hb::utils::Tr("Видалити цей документ?"));
        msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
        msgBox.setDefaultButton(QMessageBox::No);

        if (msgBox.exec() == QMessageBox::Yes)
        {
            hb::core::DocumentPtr doc = m_documentsModel.GetDocumentItemPtr(indexes[0].row());

            if (doc && hb::core::Engine::GetInstance().DeleteDocument(doc->Id()))
            {
                ApplyDocumentsFilter();
            }
        }
    }
}

void MainWindow::documentsTableView_selectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    SetButtonsEnabled();
}

void MainWindow::on_movementButton_clicked()
{
    CreateDocument(hb::core::DocumentType::Movement);
}
