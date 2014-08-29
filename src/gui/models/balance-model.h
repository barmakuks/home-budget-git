#ifndef BALANCEMODEL_H
#define BALANCEMODEL_H

#include <QAbstractItemModel>

#include "account.h"
#include "balance.h"
#include "currency.h"

namespace hb
{
namespace core
{
class IStorage;
}
}

class BalanceModel: public QAbstractItemModel
{
public:
    BalanceModel(hb::core::IStorage &storage, const std::string& date);

    // QAbstractItemModel interface
public:
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex& child) const;
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;

private:
    hb::core::AccountMapPtr     m_accounts;
    hb::core::CurrencyMapPtr    m_currencies;
    hb::core::BalanceSetPtr     m_balance;

    hb::core::IStorage&  m_storage;

};

#endif // BALANCEMODEL_H
