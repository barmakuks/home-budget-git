#ifndef ACCOUNTSMODEL_H
#define ACCOUNTSMODEL_H

#include <QAbstractListModel>
#include <vector>

#include "account.h"

class AccountsModel: public QAbstractListModel
{
public:
    AccountsModel();

    void Reload();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;

private:
    const hb::core::Account& GetAccountItem(int index) const;

    QVariant GetCellString(const QModelIndex& index) const;
    QVariant GetCellForecolor(const QModelIndex& index) const;
    QVariant GetCellBackColor(const QModelIndex& index) const;

private:
    std::vector<hb::core::AccountPtr>  m_accounts;
};

#endif // ACCOUNTSMODEL_H
