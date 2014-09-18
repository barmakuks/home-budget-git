#ifndef DOCUMENTSMODEL_H
#define DOCUMENTSMODEL_H

#include <QAbstractTableModel>

#include "account.h"
#include "currency.h"
#include "document.h"

class DocumentsModel: public QAbstractTableModel
{
public:
    DocumentsModel();

    void Reload(const QDate& minDate,
                const QDate& maxDate,
                const hb::AccountId accountId,
                const hb::CurrencyId currencyId);

    void Reload(const std::string& minDate,
                const std::string& maxDate,
                const hb::AccountId accountId,
                const hb::CurrencyId currencyId);

    // QAbstractTableModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:

    const hb::core::Document& GetDocumentItem(int index) const;

    QVariant GetCellString(const QModelIndex& index) const;
    QVariant GetCellAlignment(const QModelIndex& index) const;
    QVariant GetCellForecolor(const QModelIndex& index) const;
    QVariant GetCellBackColor(const QModelIndex& index) const;

private:

    hb::core::AccountMapPtr         m_accounts;
    hb::core::CurrencyMapPtr        m_currencies;
    hb::core::DocumentsPtr          m_documents;
    hb::core::DocumentTypeListPtr   m_docTypes;
};

#endif // DOCUMENTSMODEL_H
