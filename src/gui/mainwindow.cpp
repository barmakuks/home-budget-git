#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QDesktopWidget>
#include <QCalendarWidget>
#include <QTableView>
#include <QComboBox>
#include <iostream>
#include <QMessageBox>
#include <QGraphicsView>
#include <utility>
#include <sstream>
#include <iomanip>
#include <QGraphicsTextItem>
#include <functional>

#include "date-time-utils.h"
#include "document-dialog.h"
#include "movement-dialog.h"
#include "document.h"
#include "engine.h"
#include "convert-utils.h"
#include "currency-exchange-manager.h"
#include "models/string-format.h"
#include "report-item.h"
#include "doc-type-filter-dialog.h"

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


typedef std::function<void()> Callback;

void SetUpDate(QDateEdit* minDate, QDateEdit* maxDate, const hb::utils::DateInterval& interval, bool& stopper, Callback update_func)
{
    using namespace hb::utils;

    if (stopper)
    {
        return;
    }

    stopper = true;

    const QDate from = hb::utils::QDatefromNormalizedDate(interval.from);
    minDate->setDate(from);
    const QDate to = hb::utils::QDatefromNormalizedDate(interval.to);
    maxDate->setDate(to);

    stopper = false;

    if (update_func)
    {
        update_func();
    }
}

void SetPeriodComboBox(QComboBox* periodCB, const QDate& dateFrom, const QDate& dateTo, bool& stopper, Callback update_func)
{
    using namespace hb::utils;

    if (stopper)
    {
        return;
    }

    DateInterval interval(hb::utils::NormalizeDate(dateFrom),
                          hb::utils::NormalizeDate(dateTo));

    const DatePeriod::Period period = GetDatePeriod(interval);

    stopper = true;
    periodCB->setCurrentIndex(period);
    stopper = false;

    if (update_func)
    {
        update_func();
    }
}

}

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_currenciesReportModel(false),
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
    ui->balanceTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->reportSubItemsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->paymentsBalanceTableView->setModel(&m_paymentsBalanceModel);
//    ui->paymentsTableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->paymentsTableView->setModel(&m_paymentsModel);

    ui->documentsTableView->setModel(&m_documentsModel);
    ui->documentsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    const int m = 25;

    ui->documentsTableView->setColumnWidth(0, m * 3.5);
    ui->documentsTableView->setColumnWidth(1, m * 8);
    ui->documentsTableView->setColumnWidth(2, m * 4);
    ui->documentsTableView->setColumnWidth(3, m * 4);
    ui->documentsTableView->setColumnWidth(4, m * 8);
    ui->documentsTableView->setColumnWidth(5, m * 8);
    ui->documentsTableView->setColumnWidth(6, m * 4);
    ui->documentsTableView->setColumnWidth(7, m * 4);

    ui->accountDocsCB->setModel(&m_accountsModel);
    ui->settingsAccountsView->setModel(&m_accountsModel);

    ui->currencyDocsCB->setModel(&m_currenciesDocModel);
    ui->currencyReportCB->setModel(&m_currenciesReportModel);

    ui->startDateDocsEdit->setDate(QDate::currentDate());
    ui->endDateDocsEdit->setDate(QDate::currentDate());

    ui->summaLabel->setHidden(true);

    ui->reportSubItemsTableView->setModel(&m_reportModel);

    ui->startDateReportEdit->setDate(QDate::currentDate());
    ui->endDateReportEdit->setDate(QDate::currentDate());

    m_filterSetupInProgress = false;

    ApplyDocumentsFilter();

    connect(ui->documentsTableView->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
            this,
            SLOT(documentsTableView_selectionChanged(const QItemSelection&, const QItemSelection&)));

    connect(ui->settingsAccountsView->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
            this,
            SLOT(settingsAccountsView_selectionChanged(const QItemSelection&, const QItemSelection&)));

    m_docTypeModel.Reload();
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

void MainWindow::on_periodDocsCB_currentIndexChanged(int index)
{
    using namespace hb::utils;
    SetUpDate(ui->startDateDocsEdit,
              ui->endDateDocsEdit,
              GetDateInterval(static_cast<DatePeriod::Period>(index)),
              m_filterSetupInProgress,
              std::bind(&MainWindow::ApplyDocumentsFilter, this));
}

void MainWindow::ApplyDocumentsFilter()
{
    hb::AccountId accountId = m_accountsModel.GetAccountItemId(ui->accountDocsCB->currentIndex());
    hb::CurrencyId currencyId = m_currenciesDocModel.GetCurrencyItemId(ui->currencyDocsCB->currentIndex());

    QModelIndexList selection = ui->documentsTableView->selectionModel()->selectedRows();

    m_documentsModel.Reload(ui->startDateDocsEdit->date(),
                            ui->endDateDocsEdit->date(),
                            accountId,
                            currencyId,
                            m_docTypeModel.GetSelected());

    m_paymentsModel.Reload(ui->startDateDocsEdit->date(),
                           ui->endDateDocsEdit->date());

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

void MainWindow::CalculateSelectedRows()
{
    using namespace hb::core;

    QModelIndexList indexes = ui->documentsTableView->selectionModel()->selectedRows();

    ui->summaLabel->setHidden(indexes.size() <= 1);

    typedef std::map<hb::CurrencyId, std::pair<hb::Money, hb::Money> > DayBalance; // currency Id, <income, outcome>
    DayBalance day_balance;

    for (QModelIndexList::iterator it = indexes.begin(); it != indexes.end(); ++it)
    {
        DocumentPtr doc = m_documentsModel.GetDocumentItemPtr(it->row());

        if (doc)
        {
            if (doc->AmountTo().is_initialized() && !doc->AmountFrom().is_initialized())
            {
                const Amount& amount = doc->AmountTo().get();

                if (day_balance.find(amount.Currency()) == day_balance.end())
                {
                    day_balance.insert(std::make_pair(amount.Currency(),  std::make_pair(amount.Value(), 0)));
                }
                else
                {
                    DayBalance::mapped_type balance = day_balance[amount.Currency()];
                    day_balance[amount.Currency()] = std::make_pair(balance.first + amount.Value(), balance.second);
                }
            }
            else if (!doc->AmountTo().is_initialized() && doc->AmountFrom().is_initialized())
            {
                const Amount& amount = doc->AmountFrom().get();

                if (day_balance.find(amount.Currency()) == day_balance.end())
                {
                    day_balance.insert(std::make_pair(amount.Currency(),  std::make_pair(0, amount.Value())));
                }
                else
                {
                    DayBalance::mapped_type balance = day_balance[amount.Currency()];
                    day_balance[amount.Currency()] = std::make_pair(balance.first, amount.Value() + balance.second);
                }
            }
        }
    }

    std::stringstream balance_str;

    for (DayBalance::iterator it = day_balance.begin(); it != day_balance.end() ; ++it)
    {
        hb::core::CurrencyPtr currency = Engine::GetInstance().GetCurrencies()->at(it->first);

        balance_str << std::setfill(' ') << std::setw(15) << hb::utils::FormatMoney(-it->second.second) <<  " " << currency->Symbol();
        balance_str << "  :" << std::setfill(' ') << std::setw(15) << hb::utils::FormatMoney(it->second.first) <<  " " << currency->Symbol() << std::endl;
    }

    ui->summaLabel->setText(hb::utils::Tr(balance_str.str()));
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
    { // Main tab
        const bool enabled = ui->documentsTableView->selectionModel()->selectedRows().size() == 1;
        ui->editButton->setEnabled(enabled);
        ui->removeButton->setEnabled(enabled);
    }

    { // Settings tab
        const auto rows = ui->settingsAccountsView->selectionModel()->selectedRows();
        const bool is_one_row = rows.size() == 1 && rows.begin()->row() != 0;

        ui->btnAccountUp->setEnabled(is_one_row && rows.begin()->row() != 1);
        ui->btnAccountDown->setEnabled(is_one_row && rows.begin()->row() != m_accountsModel.rowCount() - 1);
        ui->btnAccountAdd->setEnabled(true);
        ui->btnAccountEdit->setEnabled(is_one_row);
        ui->btnAccountRemove->setEnabled(is_one_row);
    }
}

void MainWindow::on_startDateDocsEdit_dateChanged(const QDate& /*date*/)
{
    SetPeriodComboBox(ui->periodDocsCB,
                      ui->startDateDocsEdit->date(),
                      ui->endDateDocsEdit->date(),
                      m_filterSetupInProgress,
                      std::bind(&MainWindow::ApplyDocumentsFilter, this));
}

void MainWindow::on_endDateDocsEdit_dateChanged(const QDate& /*date*/)
{
    SetPeriodComboBox(ui->periodDocsCB,
                      ui->startDateDocsEdit->date(),
                      ui->endDateDocsEdit->date(),
                      m_filterSetupInProgress,
                      std::bind(&MainWindow::ApplyDocumentsFilter, this));
}

void MainWindow::on_accountDocsCB_currentIndexChanged(int /*index*/)
{
    if (!m_filterSetupInProgress)
    {
        ApplyDocumentsFilter();
    }
}

void MainWindow::on_currencyDocsCB_currentIndexChanged(int index)
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
    CalculateSelectedRows();
}

void MainWindow::settingsAccountsView_selectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
    SetButtonsEnabled();
}

void MainWindow::on_movementButton_clicked()
{
    CreateDocument(hb::core::DocumentType::Movement);
}

void MainWindow::MakeReport()
{
    using namespace hb::core;

    ReportPtr currentReport = Engine::GetInstance().GetReport(hb::utils::NormalizeDate(ui->startDateReportEdit->date()),
                              hb::utils::NormalizeDate(ui->endDateReportEdit->date()));

    const hb::CurrencyId currencyId = m_currenciesReportModel.GetCurrencyItemId(ui->currencyReportCB->currentIndex());
    m_reportModel.SetReport(currentReport, currencyId);

    if (!currentReport)
    {
        ui->reportView->setScene(nullptr);
        return;
    }

    UpdateReport();
}

void MainWindow::UpdateReport()
{
    using namespace hb::core;

    const ReportItem& main_report_item = m_reportModel.CurrentItemRef();

    hb::CurrencyId currencyId = m_currenciesReportModel.GetCurrencyItemId(ui->currencyReportCB->currentIndex());

    const auto& amount = main_report_item.Amounts().find(currencyId);

    if (amount != main_report_item.Amounts().end())
    {
        QGraphicsScene* scene = CreateReportScene(main_report_item, amount->second.CurCode());
        ui->reportView->setScene(scene);
        ui->reportView->show();
    }
    else
    {
        ui->reportView->setScene(nullptr);
    }
}

QGraphicsScene* MainWindow::CreateReportScene(const hb::core::ReportItem& report_item, hb::CurrencyId currency)
{
    using namespace hb::core;

    const qreal radius = 200.;

    std::unique_ptr<QGraphicsScene> scene(new QGraphicsScene(this));

    typedef std::vector<std::pair<std::string, hb::Money> > ReportLines;
    ReportLines values;

    hb::Money summa = 0;

    std::string currency_symbol;

    {
        // add current amount
        const auto amounts = report_item.Amounts();
        const auto cur_amount = amounts.find(currency);

        if (cur_amount != amounts.end())
        {
            const hb::Money amount = std::abs(cur_amount->second.Amount());
            const hb::Money sub_amount = std::abs(cur_amount->second.SubAmount());

            const std::string caption = report_item.DocTypeName() + ": " + hb::utils::FormatMoney(amount + sub_amount) + " " + cur_amount->second.CurName();
            currency_symbol = cur_amount->second.CurSymbol();

            QFont font;
            font.setPixelSize(16);

            QGraphicsTextItem* text = scene->addText(caption.c_str(), font);
            text->setPos(-text->boundingRect().width() / 2, -radius - text->boundingRect().height() * 2.);

            values.push_back(ReportLines::value_type(report_item.DocTypeName(), amount));
            summa += amount;
        }
        else
        {
            return nullptr;
        }
    }

    // add sub-amounts
    for (auto it : report_item.SubItems())
    {
        ReportItemPtr sub_item = it.second;
        const auto amounts = sub_item->Amounts();
        const auto cur_amount = amounts.find(currency);

        if (cur_amount != amounts.end())
        {
            hb::Money amount = std::abs(cur_amount->second.Amount()) + std::abs(cur_amount->second.SubAmount());
            values.push_back(ReportLines::value_type(sub_item->DocTypeName(), amount));
            summa += amount;
        }
    }


    QRectF rect(-radius, -radius, 2 * radius, 2 * radius);
    scene->addEllipse(-1, -1, 1, 1);

    qreal start = 0.;

    int h = random() * 360. / RAND_MAX;

    for (auto value : values)
    {
        if (value.second == 0.)
        {
            continue;
        }

        QPainterPath* path = new QPainterPath();
        qreal angle = 360.0 * value.second / summa;
        path->arcTo(rect, start, angle);

        start += angle;
        path->closeSubpath();
        QColor color;
        h = static_cast<int>(h + 360. * 0.618033988749895) % 360;
        color.setHsv(h, 255 * 0.5, 0.99 * 255, 255);

        scene->addPath(*path, QColor(color.red(), color.green(), color.blue(), 127), color);

        const qreal alpha = -3.14159265 * (start - angle / 2) / 180.0;
        const qreal xS = cos(alpha) * radius * 1;
        const qreal yS = sin(alpha) * radius * 1;

        const qreal xE = cos(alpha) * radius * 1.05;
        const qreal yE = sin(alpha) * radius * 1.05; //xS > 0 ? radius + 20 : -radius - 20;

        scene->addLine(xS, yS, xE, yE, QColor(0, 0, 0, 64));

        std::stringstream str;
        str << value.first << " ";
        str << std::setw(2) << std::fixed <<  hb::utils::FormatMoney(value.second) << currency_symbol;
        str << " (" << std::setw(2) << std::fixed << std::setprecision(1) << value.second * 100. / summa << "%)";
        QGraphicsTextItem* text = scene->addText(str.str().c_str());

        QFont font = text->font();
        font.setPixelSize(9);
        text->setFont(font);
        const qreal xT = xE > 0 ? xE : xE - text->boundingRect().width();
        const qreal yT = yE - text->boundingRect().height() / 2;
        text->setPos(xT, yT);
    }

    return scene.release();
}

void MainWindow::on_periodReportCB_currentIndexChanged(int index)
{
    SetUpDate(ui->startDateReportEdit,
              ui->endDateReportEdit,
              GetDateInterval(static_cast<hb::utils::DatePeriod::Period>(index)),
              m_filterSetupInProgress,
              std::bind(&MainWindow::MakeReport, this));
}

void MainWindow::on_currencyReportCB_currentIndexChanged(int index)
{
    MakeReport();
}

void MainWindow::on_btnReportBack_clicked()
{
    if (m_reportModel.Back())
    {
        UpdateReport();
    }
}

void MainWindow::on_reportSubItemsTableView_doubleClicked(const QModelIndex& index)
{
    QModelIndexList indexes = ui->reportSubItemsTableView->selectionModel()->selectedRows();

    if (m_reportModel.SetCurrentItem(indexes[0].row()))
    {
        UpdateReport();
    }
}

void MainWindow::on_reportsTabWidget_currentChanged(int index)
{
    switch (index)
    {
    case 0:
        ui->currencyReportCB->setEnabled(true);
        ui->intervalReportCB->setEnabled(false);
        break;

    case 1:
        ui->currencyReportCB->setEnabled(false);
        ui->intervalReportCB->setEnabled(true);
        break;

    case 2:
        ui->currencyReportCB->setEnabled(false);
        ui->intervalReportCB->setEnabled(true);
        break;
    }
}

void MainWindow::on_btnDocTypeFilter_clicked()
{
    if (DocTypeFilterDialog::ShowFilter(&m_docTypeModel))
    {
        ApplyDocumentsFilter();
    }
}

void MainWindow::on_startDateReportEdit_dateChanged(const QDate& date)
{
    SetPeriodComboBox(ui->periodReportCB,
                      date,
                      ui->endDateReportEdit->date(),
                      m_filterSetupInProgress,
                      std::bind(&MainWindow::MakeReport, this));
}

void MainWindow::on_endDateReportEdit_dateChanged(const QDate& date)
{
    SetPeriodComboBox(ui->periodReportCB,
                      ui->startDateReportEdit->date(),
                      date,
                      m_filterSetupInProgress,
                      std::bind(&MainWindow::MakeReport, this));
}

void MainWindow::on_btnAccountUp_clicked()
{
    QModelIndexList selection = ui->settingsAccountsView->selectionModel()->selectedRows();

    if (selection.size() == 1)
    {
        int selectedRow = selection.begin()->row();
        hb::AccountId accountId = m_accountsModel.GetAccountItemId(selectedRow);

        if (m_accountsModel.MoveAccountUp(accountId))
        {
            QModelIndex index = m_accountsModel.index(selectedRow - 1);
            ui->settingsAccountsView->setCurrentIndex(index);
        }
    }
}

void MainWindow::on_btnAccountDown_clicked()
{
    QModelIndexList selection = ui->settingsAccountsView->selectionModel()->selectedRows();

    if (selection.size() == 1)
    {
        int selectedRow = selection.begin()->row();
        hb::AccountId accountId = m_accountsModel.GetAccountItemId(selectedRow);

        if (m_accountsModel.MoveAccountDown(accountId))
        {
            QModelIndex index = m_accountsModel.index(selectedRow + 1);
            ui->settingsAccountsView->setCurrentIndex(index);
        }
    }
}
