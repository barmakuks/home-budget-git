#include "doc-type-filter-dialog.h"
#include "ui_doc-type-filter-dialog.h"
#include "models/doc-type-filter-model.h"

#include "memory"

bool DocTypeFilterDialog::ShowFilter(DocTypeFilterModel* model)
{
    static std::unique_ptr<DocTypeFilterDialog> dlg = nullptr;

    if (!dlg)
    {
        dlg.reset(new DocTypeFilterDialog(NULL));
        dlg->m_model = model;
        dlg->ui->docTypesLV->setModel(model);
    }

    const int result = dlg->exec();

    return  result == QDialog::Accepted;
}

DocTypeFilterDialog::DocTypeFilterDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::DocTypeFilterDialog)
{
    ui->setupUi(this);
}

DocTypeFilterDialog::~DocTypeFilterDialog()
{
    delete ui;
}

void DocTypeFilterDialog::on_buttonBox_clicked(QAbstractButton* button)
{
    if (ui->buttonBox->buttonRole(button) == QDialogButtonBox::ButtonRole::ResetRole && m_model)
    {
        m_model->ResetSelection();
    }
}
