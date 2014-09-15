#ifndef DOCUMENTSMODEL_H
#define DOCUMENTSMODEL_H

#include <QAbstractItemModel>

#include "account.h"
#include "currency.h"
#include "document.h"

class DocumentsModel: public QAbstractItemModel
{
public:
    DocumentsModel();

    void Reload(const QDate& minDate, const QDate& maxDate);
    void Reload(const std::string& minDate, const std::string& maxDate);

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

private:

    const hb::core::Document& GetDocumentItem(int index) const;

    QVariant GetCellString(const QModelIndex& index) const;
    QVariant GetCellAlignment(const QModelIndex& index) const;
    QVariant GetCellForecolor(const QModelIndex& index) const;
    QVariant GetCellBackColor(const QModelIndex& index) const;

private:

    hb::core::AccountMapPtr     m_accounts;
    hb::core::CurrencyMapPtr    m_currencies;
    hb::core::DocumentsPtr      m_documents;
};

#endif // DOCUMENTSMODEL_H
