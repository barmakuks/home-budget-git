#ifndef ACCOUNTSMODEL_H
#define ACCOUNTSMODEL_H

#include <QAbstractListModel>
#include <vector>

#include "account.h"

class AccountsModel: public QAbstractListModel
{
public:
    AccountsModel(bool allAccountsFirst = true);

    void Reload();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role) const;

    const hb::AccountId GetAccountItemId(int index) const;
    int GetIndexOfAccount(hb::AccountId accountId) const;

    bool MoveAccountDown(hb::AccountId accountId);
    bool MoveAccountUp(hb::AccountId accountId);

private:
    const hb::core::Account& GetAccountItem(int index) const;
    void SwapAccounts(hb::core::AccountList& accounts, int index_a, int index_b);


    QVariant GetCellString(const QModelIndex& index) const;
    QVariant GetCellForecolor(const QModelIndex& index) const;
    QVariant GetCellBackColor(const QModelIndex& index) const;

private:
    const int                   m_accountsStartIndex;
    hb::core::AccountListPtr    m_accounts;
};

#endif // ACCOUNTSMODEL_H
