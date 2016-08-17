#pragma once
#include <QAbstractTableModel>

#include <map>

#include "payments-balance.h"

namespace hb
{
class IStorage;
class PaymentsBalanceRow;
}

class PaymentsBalanceModel : public QAbstractTableModel
{
public:
    PaymentsBalanceModel();

    void Recalculate(const QDate& date);
    void Recalculate(const std::string& date);

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex& parent) const;
    virtual int columnCount(const QModelIndex& parent) const;
    virtual QVariant data(const QModelIndex& index, int role) const;

private:
    const hb::PaymentsBalanceRow& GetBalanceItem(int index) const;

    QVariant GetCellString(const QModelIndex& index) const;
    QVariant GetCellAlignment(const QModelIndex& index) const;
    QVariant GetCellForecolor(const QModelIndex& index) const;
    QVariant GetCellBackColor(const QModelIndex& index) const;

private:
    hb::PaymentsBalancePtr m_balance;
};

