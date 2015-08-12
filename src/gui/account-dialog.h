#ifndef ACCOUNTDIALOG_H
#define ACCOUNTDIALOG_H

#include <QDialog>
#include "account.h"
#include "models/currencies-model.h"

namespace Ui
{
class AccountDialog;
}

class AccountDialog : public QDialog
{
    Q_OBJECT

public:
    /** Open dialog to edit account
        @param document Account to edit
        @return true if account was changed successfully
    */
    static bool EditAccount(const hb::core::AccountPtr& account);

    ~AccountDialog();
protected:
    explicit AccountDialog(QWidget* parent = 0);

    /** Set data from document to UI controls */
    void SetupUI(const hb::core::AccountPtr& account);

    /** Reads data from UI controls and set them into m_document*/
    bool GetDataFromUI();

private:

    QColor GetAccountColorUI() const;
    void SetAccountColorUI(QColor color);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_toolButton_clicked();

private:
    Ui::AccountDialog*      ui;

    hb::core::AccountPtr    m_account;
    CurrenciesModel         m_currencyModel;
};

#endif // ACCOUNTDIALOG_H
