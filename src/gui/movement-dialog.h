#ifndef MOVEMENTDIALOG_H
#define MOVEMENTDIALOG_H

#include <QDialog>

#include "models/doc-type-model.h"
#include "models/accounts-model.h"
#include "models/currencies-model.h"
#include "document.h"

namespace Ui
{
class MovementDialog;
}

class MovementDialog : public QDialog
{
    Q_OBJECT

public:
    ~MovementDialog();

    /** Open dialog to edit document
      @param document Document to edit
      @return true if document was changed successfully
    */
    static bool EditDocument(const hb::core::DocumentPtr& document);

    /** Creates new empty document and opens dialog to edit it
      @param docType Type of document
      @return true if document was created and changed successfully
    */
    static bool CreateDocument(hb::core::DocumentType::TypeSign docType);

protected:
    explicit MovementDialog(QWidget* parent = 0);

    /** Set data from document to UI controls */
    void SetupUI(const hb::core::DocumentPtr& document);

    /** Reads data from UI controls and set them into m_document*/
    bool GetDataFromUI();
private slots:
    void on_okButton_clicked();

    void on_moreButton_clicked();

    void on_cancelButton_clicked();

private:

private:
    Ui::MovementDialog* ui;

    struct DialogResult
    {
        enum {Canceled = 0, Accepted, More};
    };

    DocTypeModel    m_docTypesModel;
    AccountsModel   m_accountsModel;
    CurrenciesModel m_currencyModel;
    hb::core::DocumentPtr m_document;

private:
    static MovementDialog*  dlg;

};

#endif // MOVEMENTDIALOG_H
