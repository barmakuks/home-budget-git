#include "payments-balance-model.h"

#include <QDate>
#include <QColor>

#include "payments-balance.h"
#include "utils/convert-utils.h"
#include "string-format.h"
#include "engine.h"
#include "get-doc-engine.h"

PaymentsBalanceModel::PaymentsBalanceModel()
{
    Recalculate(QDate::currentDate());
}

void PaymentsBalanceModel::Recalculate(const QDate &date)
{
    Recalculate(hb::qt_utils::NormalizeDate(date));
}

void PaymentsBalanceModel::Recalculate(const std::string &date)
{
    beginResetModel();
    using namespace hb;
    m_balance = GetDocEngine().GetPaymentsBalance(date);

    endResetModel();
}

int PaymentsBalanceModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_balance ? m_balance->size() : 0;
}

int PaymentsBalanceModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant PaymentsBalanceModel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
    case Qt::TextAlignmentRole :
    {
        return GetCellAlignment(index);
    }
    case Qt::DisplayRole:
    {
        return GetCellString(index);
    }
    case Qt::TextColorRole:
    {
        return GetCellForecolor(index);
    }
    case Qt::BackgroundColorRole:
    {
        return GetCellBackColor(index);
    }
    default:
    {
        return QVariant();
    }
    }
}

const hb::PaymentsBalanceRow &PaymentsBalanceModel::GetBalanceItem(int index) const
{
    return *(m_balance->at(index));
}

struct Columns{enum {Name, Amount};};

QVariant PaymentsBalanceModel::GetCellString(const QModelIndex &index) const
{
    using namespace hb::qt_utils;

    const auto& balance_row = GetBalanceItem(index.row());

    switch (index.column())
    {
    case Columns::Name:
    {
        return Tr(balance_row.PaymentName());
    }
    case Columns::Amount:
    {
        return Tr(hb::qt_utils::FormatMoney(balance_row.Amount()));
    }
    default:
    {
        return QVariant("no data");
    }
    }
}

QVariant PaymentsBalanceModel::GetCellAlignment(const QModelIndex &index) const
{
    if (index.column() == Columns::Amount)
    {
        return Qt::AlignVCenter + Qt::AlignRight;
    }
    return QVariant();
}

QVariant PaymentsBalanceModel::GetCellForecolor(const QModelIndex &index) const
{
    return QColor(0, 0, 0);
}

QVariant PaymentsBalanceModel::GetCellBackColor(const QModelIndex &index) const
{
    return QVariant();
}
