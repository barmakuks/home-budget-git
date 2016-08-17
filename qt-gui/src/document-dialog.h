#pragma once
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
    ~DocumentDialog();

    /** Open dialog to edit document
        @param document Document to edit
        @return true if document was changed successfully
    */
    static bool EditDocument(const hb::DocumentPtr& document);

    /** Creates new empty document and opens dialog to edit it
        @param docType Type of document
        @return true if document was created and changed successfully
    */
    static bool CreateDocument(hb::DocumentType::Direction docType);

protected:
    explicit DocumentDialog(QWidget* parent = 0);

    /** Set data from document to UI controls */
    void SetupUI(const hb::DocumentPtr& document);

    /** Reads data from UI controls and set them into m_document*/
    bool GetDataFromUI();

    void SelectDocType(hb::DocTypeId docTypeId);

    /** Expands tree view to set index visible*/
    void ExpandToIndex(const QModelIndex& index);

private slots:
    void on_okButton_clicked();

    void on_moreButton_clicked();

    void on_cancelButton_clicked();

    void on_shopComboBox_currentIndexChanged(int index);

    void on_accountComboBox_currentIndexChanged(int index);

    void on_docTypeTreeView_customContextMenuRequested(const QPoint& pos);

    void add_docType();
    void edit_docType();
    void remove_docType();

private:
    Ui::DocumentDialog* ui;

    struct DialogResult
    {
        enum
        {
            Canceled = 0,
            Accepted,
            More
        };
    };

    DocTypeModel m_docTypesModel;
    AccountsModel m_accountsModel;
    CurrenciesModel m_currencyModel;
    ShopsModel m_shopsModel;
    hb::DocumentPtr m_document;
    bool m_has_changes;

private:
    static DocumentDialog* dlg;
};
