#include "account-dialog.h"
#include "ui_account-dialog.h"

#include <QColorDialog>

#include "account.h"
#include "convert-utils.h"
#include "engine.h"

using namespace hb::core;

AccountDialog::AccountDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::AccountDialog),
    m_currencyModel(false)
{
    ui->setupUi(this);
}

void AccountDialog::SetupUI(const hb::core::AccountPtr& account)
{
    m_currencyModel.Reload();

    m_account = account;

    ui->txtAccountName->setText(hb::utils::Tr(account->Name()));
    ui->txtDescription->setText(hb::utils::Tr(account->Description()));
    ui->txtBank->setText(hb::utils::Tr(account->Bank()));

    SetAccountColorUI(QColor(account->ForegroundColor()));

    ui->cbCurrency->setModel(&m_currencyModel);

    if (account->DefaultCurrency())
    {
        ui->cbCurrency->setCurrentIndex(m_currencyModel.GetCurrencyIndex(account->DefaultCurrency()).row());
    }
    else
    {
        ui->cbCurrency->setCurrentIndex(0);
    }

    ui->cbxIsActive->setChecked(account->IsActive());
}

bool AccountDialog::GetDataFromUI()
{
    if (ui->txtAccountName->text().isEmpty())
    {
        return false;
    }

    if (ui->cbCurrency->currentIndex() < 0)
    {
        return false;
    }

    if (m_account->Id() == hb::EmptyId)
    {
        m_account->SetSortOrder(Engine::GetInstance().GetAccountsList()->size());
    }

    m_account->SetName(hb::utils::Convert(ui->txtAccountName->text()));
    m_account->SetDescription(hb::utils::Convert(ui->txtDescription->text()));
    m_account->SetBank(hb::utils::Convert(ui->txtBank->text()));
    const int color = (GetAccountColorUI().red() << 16) + (GetAccountColorUI().green() << 8) + GetAccountColorUI().blue();
    m_account->SetForegroundColor(color);
    m_account->SetDefaultCurrency(m_currencyModel.GetCurrencyItemId(ui->cbCurrency->currentIndex()));
    m_account->SetActive(ui->cbxIsActive->isChecked());

    return true;
}

QColor AccountDialog::GetAccountColorUI() const
{
    return ui->txtColor->palette().color(ui->txtColor->foregroundRole());
}

void AccountDialog::SetAccountColorUI(QColor color)
{
    QPalette palette = ui->txtColor->palette();
    palette.setColor(ui->txtColor->foregroundRole(), color);
    ui->txtColor->setPalette(palette);
}

bool AccountDialog::EditAccount(const hb::core::AccountPtr& account)
{
    if (!account)
    {
        return false;
    }

    static std::shared_ptr<AccountDialog> dlg(new AccountDialog(NULL));

    if (dlg)
    {
        dlg->SetupUI(account);

        const int result = dlg->exec();

        return result == DialogCode::Accepted;
    }

    return false;
}

AccountDialog::~AccountDialog()
{
    delete ui;
}

void AccountDialog::on_pushButton_clicked()
{
    if (GetDataFromUI())
    {
        Engine::GetInstance().Write(*m_account);
        done(DialogCode::Accepted);
    }
}

void AccountDialog::on_pushButton_2_clicked()
{
    done(DialogCode::Rejected);
}

void AccountDialog::on_toolButton_clicked()
{
    QColor color = GetAccountColorUI();

    QColor newColor = QColorDialog::getColor(color, this);

    if (newColor != color)
    {
        SetAccountColorUI(newColor);
    }

}
