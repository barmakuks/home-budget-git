#include "document-dialog.h"
#include "ui_document-dialog.h"

#include <QTreeView>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>

#include "engine.h"
#include "document.h"
#include "models/string-format.h"

DocumentDialog* DocumentDialog::dlg = NULL;

using namespace hb::core;

DocumentDialog::DocumentDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::DocumentDialog),
    m_accountsModel(false),
    m_currencyModel(false)
{
    ui->setupUi(this);
    ui->docTypeTreeView->setModel(&m_docTypesModel);
    ui->accountComboBox->setModel(&m_accountsModel);
    ui->shopComboBox->setModel(&m_shopsModel);
    ui->currencyComboBox->setModel(&m_currencyModel);
}

void DocumentDialog::SetupUI(const DocumentPtr& document)
{
    m_document = document;

    // setup models
    const DocumentTypeListPtr typelist = Engine::GetInstance().GetTypeList();
    const DocumentTypePtr docType = typelist->at(document->DocType());
    m_docTypesModel.Reload(docType->Sign());
    m_accountsModel.Reload();
    m_shopsModel.Reload();
    m_currencyModel.Reload();

    // setup dialog title
    if (docType->Sign() == DocumentType::Income)
    {
        this->setWindowTitle(QObject::tr("Доходы"));
    }
    else if (docType->Sign() == DocumentType::Outcome)
    {
        this->setWindowTitle(QObject::tr("Расходы"));
    }

    // set amount
    Amount& amount = docType->Sign() == DocumentType::Income ? document->AmountTo().get() : document->AmountFrom().get();
    ui->amountEdit->setText(QObject::tr(hb::utils::FormatMoney(amount.Value()).c_str()));

    // set account
    ui->accountComboBox->setCurrentIndex(m_accountsModel.GetIndexOfAccount(amount.Account()));

    // set shop
    ui->shopComboBox->setEditText(QObject::tr(document->Shop().c_str()));

    // set currency
    ui->currencyComboBox->setCurrentIndex(m_currencyModel.GetCurrencyIndex(amount.Currency()).row());

    // set doc type
    const QModelIndex docTypeIndex = m_docTypesModel.getDocTypeIndex(docType->Id());
    ui->docTypeTreeView->selectionModel()->select(docTypeIndex, QItemSelectionModel::Select);

    // set note
    ui->noteEdit->setText(QObject::tr(document->Note().c_str()));

    // set date
    const QDate date = QDate::fromString(QString::fromUtf8(document->DocDate().c_str()), "yyyyMMdd");
    ui->dateEdit->setDate(date);

    ExpandToIndex(docTypeIndex);
}

bool DocumentDialog::GetDataFromUI()
{
    using namespace hb;

    const QModelIndexList selectedDoctypes = ui->docTypeTreeView->selectionModel()->selectedIndexes();

    if (!m_document || selectedDoctypes.size() != 1)
    {
        return false;
    }

    const DocTypeId docTypeId = static_cast<DocTypeId>(selectedDoctypes.begin()->internalId());
    const DocumentTypePtr docType = Engine::GetInstance().GetTypeList()->at(docTypeId);

    Amount amount;
    amount.SetValue(ui->amountEdit->text().toDouble() * 100);
    amount.SetAccount(m_accountsModel.GetAccountItemId(ui->accountComboBox->currentIndex()));
    amount.SetCurrency(m_currencyModel.GetCurrencyItemId(ui->currencyComboBox->currentIndex()));

    if (docType->Sign() == DocumentType::Income)
    {
        m_document->SetAmountTo(amount);
        m_document->ResetAmountFrom();
    }
    else if (docType->Sign() == DocumentType::Outcome)
    {
        m_document->SetAmountFrom(amount);
        m_document->ResetAmountTo();
    }

    m_document->SetDocDate(ui->dateEdit->date().toString("yyyyMMdd").toUtf8().data());
    m_document->SetDocType(docTypeId);
    m_document->SetShop(ui->shopComboBox->currentText().toUtf8().data());
    m_document->SetNote(ui->noteEdit->text().toUtf8().data());

    return true;
}

void DocumentDialog::ExpandToIndex(const QModelIndex &index)
{
    QModelIndex parent = m_docTypesModel.parent(index);

    while (parent.isValid())
    {
        ui->docTypeTreeView->setExpanded(parent, true);
        parent = m_docTypesModel.parent(parent);
    }
}

DocumentDialog::~DocumentDialog()
{
    delete ui;
}

bool DocumentDialog::EditDocument(const DocumentPtr& document)
{
    if (!document)
    {
        return false;
    }

    if (!dlg)
    {
        dlg = new DocumentDialog(NULL);
    }

    dlg->SetupUI(document);

    const int result = dlg->exec();

    return result != DialogResult::Canceled || document != dlg->m_document;
}

bool DocumentDialog::CreateDocument(DocumentType::TypeSign docType)
{
    DocumentPtr doc = Engine::GetInstance().CreateDocument(docType);

    return EditDocument(doc);
}

void DocumentDialog::on_okButton_clicked()
{
    if (GetDataFromUI())
    {
        Engine::GetInstance().Write(*m_document);
        done(DialogResult::Accepted);
    }
}

void DocumentDialog::on_moreButton_clicked()
{
    if (GetDataFromUI())
    {
        Engine::GetInstance().Write(*m_document);
        m_document = m_document->CreateTemplate();
        ui->amountEdit->setText(QObject::tr(hb::utils::FormatMoney(0).c_str()));
        ui->shopComboBox->setEditText(QObject::tr(m_document->Shop().c_str()));
    }
}

void DocumentDialog::on_cancelButton_clicked()
{
    done(DialogResult::Canceled);
}

void DocumentDialog::on_shopComboBox_currentIndexChanged(int index)
{
    ui->shopComboBox->setEditText(QObject::tr(m_shopsModel.GetShopItemId(index).c_str()));
}

void DocumentDialog::on_accountComboBox_currentIndexChanged(int index)
{
    hb::AccountId accountId = m_accountsModel.GetAccountItemId(index);
    AccountPtr account = Engine::GetInstance().GetAccounts()->at(accountId);
    ui->currencyComboBox->setCurrentIndex(m_currencyModel.GetCurrencyIndex(account->DefaultCurrency()).row());
}
