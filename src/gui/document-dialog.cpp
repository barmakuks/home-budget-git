#include "document-dialog.h"
#include "ui_document-dialog.h"

#include <QTreeView>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>

#include "engine.h"
#include "document.h"
#include "models/string-format.h"
#include "convert-utils.h"

DocumentDialog* DocumentDialog::dlg = NULL;

using namespace hb::core;
using namespace hb::utils;

DocumentDialog::DocumentDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::DocumentDialog),
    m_accountsModel(false),
    m_currencyModel(false),
    m_has_changes(false)
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
        this->setWindowTitle(Tr("Дохiд"));
    }
    else if (docType->Sign() == DocumentType::Outcome)
    {
        this->setWindowTitle(Tr("Витрати"));
    }

    // set amount
    Amount& amount = docType->Sign() == DocumentType::Income ? document->AmountTo().get() : document->AmountFrom().get();
    ui->amountEdit->setText(Tr(FormatMoney(amount.Value())));

    // set account
    ui->accountComboBox->setCurrentIndex(m_accountsModel.GetIndexOfAccount(amount.Account()));

    // set shop
    ui->shopComboBox->setEditText(Tr(document->Shop()));

    // set currency
    ui->currencyComboBox->setCurrentIndex(m_currencyModel.GetCurrencyIndex(amount.Currency()).row());

    // set note
    ui->noteEdit->setText(Tr(document->Note()));

    // set date
    const QDate date = QDatefromNormalizedDate(document->DocDate());
    ui->dateEdit->setDate(date);

    // set doc type
    SelectDocType(docType->Id());
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

    m_document->SetDocDate(hb::utils::NormalizeDate(ui->dateEdit->date()));
    m_document->SetDocType(docTypeId);
    m_document->SetShop(Convert(ui->shopComboBox->currentText()));
    m_document->SetNote(Convert(ui->noteEdit->text()));

    return true;
}

void DocumentDialog::SelectDocType(hb::DocTypeId docTypeId)
{
    // set doc type
    const QModelIndex docTypeIndex = m_docTypesModel.getDocTypeIndex(docTypeId);
    ui->docTypeTreeView->selectionModel()->select(docTypeIndex, QItemSelectionModel::Select);

    ExpandToIndex(docTypeIndex);
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

    dlg->m_has_changes = false;
    dlg->SetupUI(document);

    const int result = dlg->exec();

    return result != DialogResult::Canceled
            || document != dlg->m_document
            || dlg->m_has_changes;
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
        ui->amountEdit->setText(Tr(hb::utils::FormatMoney(0)));
        ui->shopComboBox->setEditText(Tr(m_document->Shop()));
    }
}

void DocumentDialog::on_cancelButton_clicked()
{
    done(DialogResult::Canceled);
}

void DocumentDialog::on_shopComboBox_currentIndexChanged(int index)
{
    ui->shopComboBox->setEditText(Tr(m_shopsModel.GetShopItemId(index)));
}

void DocumentDialog::on_accountComboBox_currentIndexChanged(int index)
{
    hb::AccountId accountId = m_accountsModel.GetAccountItemId(index);
    AccountPtr account = Engine::GetInstance().GetAccounts()->at(accountId);
    ui->currencyComboBox->setCurrentIndex(m_currencyModel.GetCurrencyIndex(account->DefaultCurrency()).row());
}

void DocumentDialog::on_docTypeTreeView_customContextMenuRequested(const QPoint &pos)
{
    std::unique_ptr<QMenu> menu(new QMenu(this));
    menu->addAction(Tr("Додати"), this, SLOT(add_docType()));
    menu->addAction(Tr("Перейменувати"), this, SLOT(edit_docType()));
    menu->addAction(Tr("Видалити"), this, SLOT(remove_docType()));
    menu->exec(ui->docTypeTreeView->mapToGlobal(pos));
}

void DocumentDialog::add_docType()
{
    const QModelIndexList selectedDoctypes = ui->docTypeTreeView->selectionModel()->selectedIndexes();

    if (selectedDoctypes.size() != 1)
    {
        return;
    }
    const hb::DocTypeId parentDocTypeId = static_cast<hb::DocTypeId>(selectedDoctypes.begin()->internalId());

    if (parentDocTypeId == hb::EmptyId)
    {
        return;
    }

    const DocumentTypePtr parentDocType = Engine::GetInstance().GetTypeList()->at(parentDocTypeId);

    QInputDialog dlg;
    dlg.setLabelText(Tr("Введить назву"));
    dlg.setWindowTitle(Tr("Тип документу"));
    dlg.setTextValue(Tr(""));

    if (dlg.exec() == QInputDialog::DialogCode::Accepted)
    {

        DocumentType docType;
        docType.SetParentId(parentDocTypeId);
        docType.SetName(Convert(dlg.textValue()));
        docType.SetSign(parentDocType->Sign());

        Engine::GetInstance().Write(docType);

        Engine::GetInstance().GetTypeList(true);
        m_docTypesModel.Reload(parentDocType->Sign());

        SelectDocType(docType.Id());
    }
}

void DocumentDialog::edit_docType()
{
    const QModelIndexList selectedDoctypes = ui->docTypeTreeView->selectionModel()->selectedIndexes();

    if (selectedDoctypes.size() != 1)
    {
        return;
    }

    QInputDialog dlg;
    dlg.setLabelText(Tr("Введить назву"));
    dlg.setWindowTitle(Tr("Тип документу"));
    const hb::DocTypeId docTypeId = static_cast<hb::DocTypeId>(selectedDoctypes.begin()->internalId());
    const DocumentTypePtr docType = Engine::GetInstance().GetTypeList()->at(docTypeId);
    dlg.setTextValue(Tr(docType->Name()));

    if (dlg.exec() == QInputDialog::DialogCode::Accepted)
    {
        docType->SetName(Convert(dlg.textValue()));
        Engine::GetInstance().Write(*docType);
        m_docTypesModel.Reload(docType->Sign());
        SelectDocType(docType->Id());
    }
}

void DocumentDialog::remove_docType()
{
    const QModelIndexList selectedDoctypes = ui->docTypeTreeView->selectionModel()->selectedIndexes();

    if (selectedDoctypes.size() != 1)
    {
        return;
    }
    const hb::DocTypeId docTypeId = static_cast<hb::DocTypeId>(selectedDoctypes.begin()->internalId());
    const DocumentTypePtr docType = Engine::GetInstance().GetTypeList()->at(docTypeId);

    if (docType->ParentId() == hb::EmptyId)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(hb::utils::Tr("Видалення документу"));
        msgBox.setText(hb::utils::Tr("Видалення документу"));
        msgBox.setInformativeText(hb::utils::Tr("Неможливо видалити цей тип документу"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();

        return;
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(hb::utils::Tr("Видалення документу"));
        msgBox.setText(hb::utils::Tr("Видалення документу"));
        msgBox.setInformativeText(hb::utils::Tr("Видалити цей тип документу?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);

        if (msgBox.exec() == QMessageBox::Yes)
        {
            Engine::GetInstance().DeleteDocumentType(docType->Id());
            Engine::GetInstance().GetTypeList(true);
            m_docTypesModel.Reload(docType->Sign());

            SelectDocType(docType->ParentId());

            m_has_changes = true;
        }
    }
}
