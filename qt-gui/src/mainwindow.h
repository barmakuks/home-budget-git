#pragma once
#include <QMainWindow>

#include "models/balance-model.h"
#include "models/documents-model.h"
#include "models/accounts-model.h"
#include "models/currencies-model.h"
#include "models/payments-balance-model.h"
#include "models/payments-model.h"
#include "models/report-model.h"
#include "models/doc-type-filter-model.h"

#include "report-item.h"

namespace Ui
{
class MainWindow;
}

class DocumentDialog;
class QItemSelection;
class QGraphicsScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    void ApplyDocumentsFilter();
    void UpdateBalance();
    void EditDocument();
    void CalculateSelectedRows();

    void CreateDocument(hb::DocumentType::Direction docType);

    void SetButtonsEnabled();

    /***************** build Report routine **********************/
    QGraphicsScene* CreateReportScene(const hb::ReportItem& report_item, hb::CurrencyId currency);
    void MakeReport();
    void UpdateReport();

private slots:
    void on_calendarWidget_clicked(const QDate& date);

    void on_periodDocsCB_currentIndexChanged(int index);

    void on_startDateDocsEdit_dateChanged(const QDate& date);

    void on_endDateDocsEdit_dateChanged(const QDate& date);

    void on_accountDocsCB_currentIndexChanged(int index);

    void on_currencyDocsCB_currentIndexChanged(int index);

    void on_creditButton_clicked();

    void on_debitButton_clicked();

    void on_editButton_clicked();

    void on_documentsTableView_doubleClicked(const QModelIndex& index);

    void on_removeButton_clicked();

    void documentsTableView_selectionChanged(const QItemSelection& selected,
                                             const QItemSelection& deselected);

    void on_movementButton_clicked();

    void on_periodReportCB_currentIndexChanged(int index);

    void on_currencyReportCB_currentIndexChanged(int index);

    void on_btnReportBack_clicked();

    void on_reportSubItemsTableView_doubleClicked(const QModelIndex& index);

    void on_reportsTabWidget_currentChanged(int index);

    void on_btnDocTypeFilter_clicked();

    void on_startDateReportEdit_dateChanged(const QDate& date);

    void on_endDateReportEdit_dateChanged(const QDate& date);

private:
    Ui::MainWindow* ui;
    BalanceModel m_balanceModel;
    DocumentsModel m_documentsModel;
    AccountsModel m_accountsModel;
    CurrenciesModel m_currenciesDocModel;
    CurrenciesModel m_currenciesReportModel;
    PaymentsBalanceModel m_paymentsBalanceModel;
    PaymentsModel m_paymentsModel;
    ReportModel m_reportModel;
    DocTypeFilterModel m_docTypeModel;

    bool m_filterSetupInProgress;

    DocumentDialog* m_doc_dlg;
};
