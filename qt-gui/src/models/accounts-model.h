#pragma once
#include <QAbstractListModel>
#include <vector>

#include "account.h"

class AccountsModel : public QAbstractListModel
{
public:
    AccountsModel(bool allAccountsFirst = true);

    void Reload();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;

    const hb::AccountId GetAccountItemId(int index) const;
    int GetIndexOfAccount(hb::AccountId accountId) const;

private:
    const hb::Account& GetAccountItem(int index) const;

    QVariant GetCellString(const QModelIndex& index) const;
    QVariant GetCellForecolor(const QModelIndex& index) const;
    QVariant GetCellBackColor(const QModelIndex& index) const;

private:
    const int m_accountsStartIndex;
    hb::AccountListPtr m_accounts;
};
