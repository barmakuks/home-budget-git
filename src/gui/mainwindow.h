#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "core.h"
#include "models/balance-model.h"
#include "models/documents-model.h"
#include "models/accounts-model.h"
#include "models/currencies-model.h"
#include "models/payments-balance-model.h"

namespace Ui
{
class MainWindow;
}

class DocumentDialog;
class QItemSelection;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    void SetPeriodComboBox(const QDate& dateFrom, const QDate& dateTo);
    void ApplyDocumentsFilter();
    void UpdateBalance();
    void EditDocument();

    void CreateDocument(hb::core::DocumentType::TypeSign docType);

    void SetButtonsEnabled();

private slots:
    void on_calendarWidget_clicked(const QDate& date);

    void on_periodComboBox_currentIndexChanged(int index);

    void on_startDateEdit_dateChanged(const QDate& date);

    void on_endDateEdit_dateChanged(const QDate& date);

    void on_accountComboBox_currentIndexChanged(int index);

    void on_currencyComboBox_currentIndexChanged(int index);

    void on_creditButton_clicked();

    void on_debitButton_clicked();

    void on_editButton_clicked();

    void on_documentsTableView_doubleClicked(const QModelIndex& index);

    void on_removeButton_clicked();

    void documentsTableView_selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

    void on_movementButton_clicked();

private:
    Ui::MainWindow*         ui;
    BalanceModel            m_balanceModel;
    DocumentsModel          m_documentsModel;
    AccountsModel           m_accountsModel;
    CurrenciesModel         m_currenciesModel;
    PaymentsBalanceModel    m_paymentsBalanceModel;

    bool                    m_filterSetupInProgress;

    DocumentDialog*  m_doc_dlg;
};

#endif // MAINWINDOW_H