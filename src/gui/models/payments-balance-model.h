#ifndef PAYMENTSBALANCEMODEL_H
#define PAYMENTSBALANCEMODEL_H

#include <QAbstractTableModel>

#include <map>

#include "payments-balance.h"

namespace hb
{
namespace core
{
class IStorage;
class PaymentsBalanceRow;
}
}

class PaymentsBalanceModel: public QAbstractTableModel
{
public:
    PaymentsBalanceModel();

    void Recalculate(const QDate& date);
    void Recalculate(const std::string& date);

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

private:

    const hb::core::PaymentsBalanceRow& GetBalanceItem(int index) const;

    QVariant GetCellString(const QModelIndex& index) const;
    QVariant GetCellAlignment(const QModelIndex& index) const;
    QVariant GetCellForecolor(const QModelIndex& index) const;
    QVariant GetCellBackColor(const QModelIndex& index) const;

private:

    hb::core::PaymentsBalancePtr    m_balance;

};

#endif // PAYMENTSBALANCEMODEL_H
