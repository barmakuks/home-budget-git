#include "movement-dialog.h"
#include "ui_movement-dialog.h"

#include <QComboBox>

#include "engine.h"
#include "documenttype.h"
#include "models/string-format.h"
#include "convert-utils.h"

MovementDialog* MovementDialog::dlg = NULL;

using namespace hb::core;
using namespace hb::utils;

MovementDialog::MovementDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::MovementDialog),
    m_accountsModel(false),
    m_currencyModel(false)
{
    ui->setupUi(this);

    ui->accountFromComboBox->setModel(&m_accountsModel);
    ui->accountToComboBox->setModel(&m_accountsModel);
    ui->currencyFromComboBox->setModel(&m_currencyModel);
    ui->currencyToComboBox->setModel(&m_currencyModel);
    ui->noteComboBox->setModel(&m_docTypesModel);
}

void MovementDialog::SetupUI(const hb::core::DocumentPtr &document)
{
    m_document = document;

    // setup models
    const DocumentTypeListPtr typelist = Engine::GetInstance().GetTypeList();
    const DocumentTypePtr docType = typelist->at(document->DocType());
    m_docTypesModel.Reload(docType->Sign());
    m_accountsModel.Reload();
    m_currencyModel.Reload();

    // setup dialog title
    assert(docType->Sign() == DocumentType::Movement);

    // set amount
    Amount& amountFrom = document->AmountFrom().get();
    Amount& amountTo = document->AmountTo().get();

    ui->amountFromEdit->setText(Tr(FormatMoney(amountFrom.Value())));
    ui->amountToEdit->setText(Tr(FormatMoney(amountTo.Value())));

    // set account
    ui->accountFromComboBox->setCurrentIndex(m_accountsModel.GetIndexOfAccount(amountFrom.Account()));
    ui->accountToComboBox->setCurrentIndex(m_accountsModel.GetIndexOfAccount(amountTo.Account()));

    // set currency
    ui->currencyFromComboBox->setCurrentIndex(m_currencyModel.GetCurrencyIndex(amountFrom.Currency()).row());
    ui->currencyToComboBox->setCurrentIndex(m_currencyModel.GetCurrencyIndex(amountTo.Currency()).row());

    // set doc type
    const QModelIndex docTypeIndex = m_docTypesModel.getDocTypeIndex(docType->Id());
    ui->noteComboBox->setCurrentIndex(docTypeIndex.row());

    // set date
    const QDate date = QDatefromNormalizedDate(document->DocDate());
    ui->dateEdit->setDate(date);
}

bool MovementDialog::GetDataFromUI()
{
    using namespace hb;

    if (!m_document)
    {
        return false;
    }

    const QModelIndex index = m_docTypesModel.index(ui->noteComboBox->currentIndex(), 0, QModelIndex());
    const DocTypeId docTypeId = index.internalId();

    Amount amountFrom;
    amountFrom.SetValue(ui->amountFromEdit->text().toDouble() * 100);
    amountFrom.SetAccount(m_accountsModel.GetAccountItemId(ui->accountFromComboBox->currentIndex()));
    amountFrom.SetCurrency(m_currencyModel.GetCurrencyItemId(ui->currencyFromComboBox->currentIndex()));

    Amount amountTo;
    amountTo.SetValue(ui->amountFromEdit->text().toDouble() * 100);
    amountTo.SetAccount(m_accountsModel.GetAccountItemId(ui->accountToComboBox->currentIndex()));
    amountTo.SetCurrency(m_currencyModel.GetCurrencyItemId(ui->currencyToComboBox->currentIndex()));

    m_document->SetAmountFrom(amountFrom);
    m_document->SetAmountTo(amountTo);

    m_document->SetDocDate(hb::utils::NormalizeDate(ui->dateEdit->date()));
    m_document->SetDocType(docTypeId);
//    m_document->SetNote(Convert(ui->noteEdit->text()));

    return true;

}

MovementDialog::~MovementDialog()
{
    delete ui;
}

bool MovementDialog::EditDocument(const hb::core::DocumentPtr& document)
{
    if (!document)
    {
        return false;
    }

    if (!dlg)
    {
        dlg = new MovementDialog(NULL);
    }

    dlg->SetupUI(document);

    const int result = dlg->exec();

    return result != DialogResult::Canceled || document != dlg->m_document;
}

bool MovementDialog::CreateDocument(hb::core::DocumentType::TypeSign docType)
{
    DocumentPtr doc = Engine::GetInstance().CreateDocument(docType);

    return EditDocument(doc);
}

void MovementDialog::on_okButton_clicked()
{
    if (GetDataFromUI())
    {
        Engine::GetInstance().Write(*m_document);
        done(DialogResult::Accepted);
    }
}

void MovementDialog::on_moreButton_clicked()
{
    if (GetDataFromUI())
    {
        Engine::GetInstance().Write(*m_document);
        m_document = m_document->CreateTemplate();
        ui->amountFromEdit->setText(Tr(hb::utils::FormatMoney(0)));
        ui->amountToEdit->setText(Tr(hb::utils::FormatMoney(0)));
    }
}

void MovementDialog::on_cancelButton_clicked()
{
    done(DialogResult::Canceled);
}
