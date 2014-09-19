#ifndef DOCUMENTDIALOG_H
#define DOCUMENTDIALOG_H

#include <QDialog>

#include "models/doc-type-model.h"
#include "models/accounts-model.h"

namespace Ui {
class DocumentDialog;
}

class DocumentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DocumentDialog(QWidget *parent = 0);
    ~DocumentDialog();

    void show(hb::core::DocumentType::TypeSign sign);

private:
    void show() { QDialog::show(); }

    Ui::DocumentDialog *ui;

    DocTypeModel        m_docTypesModel;
    AccountsModel       m_accountsModel;
};

#endif // DOCUMENTDIALOG_H
