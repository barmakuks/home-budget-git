#include "document-dialog.h"
#include "ui_document-dialog.h"

#include <QTreeView>
#include <QTextEdit>
#include <QComboBox>

#include "engine.h"
#include "document.h"
#include "models/string-format.h"

DocumentDialog* DocumentDialog::dlg = NULL;

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

hb::core::DocumentPtr DocumentDialog::CreateDocument(hb::core::DocumentType::TypeSign docType)
{
    using namespace hb::core;

    DocumentPtr doc = Engine::GetInstance().CreateDocument(docType);

    if (doc && EditDocument(doc))
    {
        return doc;
    }

    return doc;
}

void DocumentDialog::SetDocument(hb::core::DocumentPtr& document)
{
    using namespace hb::core;
    const DocumentTypeListPtr typelist = Engine::GetInstance().GetTypeList();
    const DocumentTypePtr docType = typelist->at(document->DocType());
    m_docTypesModel.Reload(docType->Sign());
    m_accountsModel.Reload();
    m_shopsModel.Reload();
    m_currencyModel.Reload();

    Amount& amount = docType->Sign() == DocumentType::Income ? document->AmountTo().get() : document->AmountFrom().get();
    ui->amountEdit->setText(QObject::tr(hb::utils::FormatMoney(amount.Value()).c_str()));

    ui->accountComboBox->setCurrentIndex(m_accountsModel.GetIndexOfAccount(amount.Account()));

    ui->shopComboBox->setEditText(QObject::tr(document->Shop().c_str()));
    ui->currencyComboBox->setCurrentIndex(m_currencyModel.GetCurrencyIndex(amount.Currency()).row());

    const QModelIndex docTypeIndex = m_docTypesModel.getDocTypeIndex(docType->Id());
    ui->docTypeTreeView->selectionModel()->select(docTypeIndex, QItemSelectionModel::Select);

    ExpandToIndex(docTypeIndex);
}

void DocumentDialog::GetDocument(hb::core::DocumentPtr& document)
{
}

bool DocumentDialog::CheckRequiredFields()
{
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

bool DocumentDialog::EditDocument(hb::core::DocumentPtr& document)
{
    if (!dlg)
    {
        dlg = new DocumentDialog(NULL);
    }

    dlg->SetDocument(document);

    const int result = dlg->exec();

    if (result == DialogResult::Accepted || result == DialogResult::More)
    {
        dlg->GetDocument(document);
        return true;
    }

    return false;
}

DocumentDialog::~DocumentDialog()
{
    delete ui;
}

void DocumentDialog::on_okButton_clicked()
{
    if (CheckRequiredFields())
    {
        done(DialogResult::Accepted);
    }
}

void DocumentDialog::on_moreButton_clicked()
{
    if (CheckRequiredFields())
    {
        done(DialogResult::More);
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

