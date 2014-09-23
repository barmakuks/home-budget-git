#ifndef DOCUMENTDIALOG_H
#define DOCUMENTDIALOG_H

#include <QDialog>

#include "models/doc-type-model.h"
#include "models/accounts-model.h"
#include "models/currencies-model.h"
#include "models/shops-model.h"
#include "document.h"

namespace Ui
{
class DocumentDialog;
}

class DocumentDialog : public QDialog
{
    Q_OBJECT

public:
    struct DialogResult
    {
        enum {Canceled = 0, Accepted, More};
    };

public:
    explicit DocumentDialog(QWidget* parent = 0);
    ~DocumentDialog();

    static bool EditDocument(hb::core::DocumentPtr& document);
    static hb::core::DocumentPtr CreateDocument(hb::core::DocumentType::TypeSign docType);

protected:
    void SetDocument(hb::core::DocumentPtr& document);
    void GetDocument(hb::core::DocumentPtr& document);

    bool CheckRequiredFields();

    void ExpandToIndex(const QModelIndex& index);

private slots:
    void on_okButton_clicked();

    void on_moreButton_clicked();

    void on_cancelButton_clicked();

    void on_shopComboBox_currentIndexChanged(int index);

private:
    void show()
    {
        QDialog::show();
    }

    Ui::DocumentDialog* ui;

    DocTypeModel    m_docTypesModel;
    AccountsModel   m_accountsModel;
    CurrenciesModel m_currencyModel;
    ShopsModel      m_shopsModel;
//    hb::core::DocumentPtr   m_document;

private:
    static DocumentDialog*  dlg;
};

#endif // DOCUMENTDIALOG_H
