#pragma once
#include <QAbstractItemModel>

#include "account.h"
#include "balance.h"
#include "currency.h"

class BalanceModel : public QAbstractItemModel
{
public:
    BalanceModel();

    void Recalculate(const QDate& date);
    void Recalculate(const std::string& date);

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex& parent) const;
    QModelIndex parent(const QModelIndex& child) const;
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;

private:
    const hb::BalanceRow& GetBalanceItem(int index) const;

    QVariant GetCellString(const QModelIndex& index) const;
    QVariant GetCellAlignment(const QModelIndex& index) const;
    QVariant GetCellForecolor(const QModelIndex& index) const;
    QVariant GetCellBackColor(const QModelIndex& index) const;

private:
    hb::AccountMapPtr m_accounts;
    hb::CurrencyMapPtr m_currencies;
    hb::BalancePtr m_balance;
    hb::TotalBalancePtr m_total_balance;
};
