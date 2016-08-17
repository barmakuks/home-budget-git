#pragma once
#include <QAbstractTableModel>

#include "account.h"
#include "currency.h"
#include "document.h"

class DocumentsModel : public QAbstractTableModel
{
public:
    DocumentsModel();

    void Reload(const QDate& minDate,
                const QDate& maxDate,
                const hb::AccountId accountId,
                const hb::CurrencyId currencyId,
                const hb::DocTypeIdList& doc_types);

    void Reload(const std::string& minDate,
                const std::string& maxDate,
                const hb::AccountId accountId,
                const hb::CurrencyId currencyId,
                const hb::DocTypeIdList& doc_types);

    const hb::DocumentPtr& GetDocumentItemPtr(int index) const;

    // QAbstractTableModel interface
public:
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    const hb::Document& GetDocumentItemRef(int index) const;

    QVariant GetCellString(const QModelIndex& index) const;
    QVariant GetCellAlignment(const QModelIndex& index) const;
    QVariant GetCellForecolor(const QModelIndex& index) const;
    QVariant GetCellBackColor(const QModelIndex& index) const;

private:
    hb::AccountMapPtr m_accounts;
    hb::CurrencyMapPtr m_currencies;
    hb::DocumentsPtr m_documents;
};
