#include "document-dialog.h"
#include "ui_document-dialog.h"

#include <QTreeView>

#include "model.h"

DocumentDialog::DocumentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DocumentDialog),
    m_accountsModel(false)
{
    ui->setupUi(this);
    ui->docTypeTreeView->setModel(&m_docTypesModel);
    ui->accountComboBox->setModel(&m_accountsModel);
}

void DocumentDialog::show(hb::core::DocumentType::TypeSign sign)
{
    m_docTypesModel.Reload(sign);
    m_accountsModel.Reload();
    QDialog::show();
}

DocumentDialog::~DocumentDialog()
{
    delete ui;
}
