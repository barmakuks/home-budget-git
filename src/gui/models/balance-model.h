#ifndef BALANCEMODEL_H
#define BALANCEMODEL_H

#include <QAbstractItemModel>

#include "account.h"
#include "balance.h"
#include "currency.h"
#include <map>

namespace hb
{
namespace core
{
class IStorage;
class BalanceRow;
}
}

class BalanceModel: public QAbstractItemModel
{
public:
    BalanceModel(hb::core::IStorage &storage);

    void Recalculate(const QDate& date);
    void Recalculate(const std::string& date);

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex& child) const;
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:

    const hb::core::BalanceRow& GetBalanceItem(int index) const;

    QVariant GetCellString(const QModelIndex& index) const;
    QVariant GetCellAlignment(const QModelIndex& index) const;
    QVariant GetCellForecolor(const QModelIndex& index) const;
    QVariant GetCellBackColor(const QModelIndex& index) const;

private:

    hb::core::AccountMapPtr     m_accounts;
    hb::core::CurrencyMapPtr    m_currencies;
    hb::core::BalancePtr        m_balance;
    hb::core::TotalBalancePtr   m_total_balance;

    hb::core::IStorage&         m_storage;

};

#endif // BALANCEMODEL_H
