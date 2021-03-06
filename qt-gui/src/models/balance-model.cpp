#include "balance-model.h"

#include <QColor>
#include <QDate>

#include "engine.h"
#include "get-doc-engine.h"
#include "string-format.h"
#include "balance.h"
#include "utils/convert-utils.h"

BalanceModel::BalanceModel()
{
    auto& model = hb::GetDocEngine();

    m_accounts = model.GetAccounts();

    m_currencies = model.GetCurrencies();

    Recalculate(QDate::currentDate());
}

void BalanceModel::Recalculate(const QDate& date)
{
    Recalculate(hb::qt_utils::NormalizeDate(date));
}

namespace
{
class AccountOrderComporator
{
public:
    AccountOrderComporator(const hb::AccountMapPtr& accounts)
        : m_accounts(accounts)
    {
    }

    bool operator()(const hb::BalanceRowPtr& first, const hb::BalanceRowPtr& second)
    {
        if (first->Account() == second->Account())
        {
            return first->Currency() > second->Currency();
        }
        else
        {
            const auto first_account
                = first->Account() != hb::EmptyId ? m_accounts->at(first->Account()) : NULL;
            const auto second_account
                = second->Account() != hb::EmptyId ? m_accounts->at(second->Account()) : NULL;

            if (first_account && second_account)
            {
                return first_account->SortOrder() < second_account->SortOrder();
            }
            else
            {
                return second_account != NULL;
            }
        }
    }

private:
    hb::AccountMapPtr m_accounts;
};
}

void BalanceModel::Recalculate(const std::string& date)
{
    beginResetModel();
    m_balance = hb::GetDocEngine().GetBalance(date);
    m_total_balance = m_balance->GetTotalBalance();

    AccountOrderComporator comporator(m_accounts);

    std::sort(m_balance->begin(), m_balance->end(), comporator);
    std::sort(m_total_balance->begin(), m_total_balance->end(), comporator);

    endResetModel();
}

QModelIndex BalanceModel::index(int row, int column, const QModelIndex& parent) const
{
    return createIndex(row, column);
}

QModelIndex BalanceModel::parent(const QModelIndex& child) const
{
    return QModelIndex();
}

namespace
{
struct Columns
{
    enum
    {
        Account = 0,
        Amount,
        Currency,
        Count
    };
};
}

int BalanceModel::rowCount(const QModelIndex& /*parent*/) const
{
    int row_count = (m_balance) ? m_balance->size() : 0;
    row_count += (m_total_balance) ? m_total_balance->size() : 0;
    return row_count;
}

int BalanceModel::columnCount(const QModelIndex& /*parent*/) const
{
    return Columns::Count;
}

Qt::ItemFlags BalanceModel::flags(const QModelIndex& /*index*/) const
{
    return Qt::ItemIsEnabled;  // | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable |
                               // Qt::ItemIsEditable;
}

QVariant BalanceModel::data(const QModelIndex& index, int role) const
{
    switch (role)
    {
    case Qt::TextAlignmentRole:
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

const hb::BalanceRow& BalanceModel::GetBalanceItem(int index) const
{
    static hb::BalanceRow fake_balance;

    if (index < 0 || !m_balance)
    {
        return fake_balance;
    }

    if (static_cast<size_t>(index) >= m_balance->size())
    {
        return *(m_total_balance->at(index - m_balance->size()));
    }
    else
    {
        return *(m_balance->at(index));
    }
}

QVariant BalanceModel::GetCellString(const QModelIndex& index) const
{
    using namespace hb::qt_utils;

    const auto& prev_balance = GetBalanceItem(index.row() - 1);
    const auto& balance = GetBalanceItem(index.row());

    switch (index.column())
    {
    case Columns::Account:
    {
        hb::AccountId accountId = balance.Account();
        if (accountId != hb::EmptyId && prev_balance.Account() != accountId)
        {
            const auto& account = m_accounts->at(accountId);

            return Tr(hb::qt_utils::FormatAccountName(*account));
        }

        return QVariant();
    }
    case Columns::Amount:
    {
        return Tr(hb::qt_utils::FormatMoney(balance.Amount()));
    }
    case Columns::Currency:
    {
        return Tr(m_currencies->at(balance.Currency())->Symbol());
    }
    default:
    {
        return QVariant("no data");
    }
    }
}

QVariant BalanceModel::GetCellAlignment(const QModelIndex& index) const
{
    if (index.column() == Columns::Amount)
    {
        return Qt::AlignVCenter + Qt::AlignRight;
    }
    return QVariant();
}

QVariant BalanceModel::GetCellBackColor(const QModelIndex& index) const
{
    if (static_cast<size_t>(index.row()) >= m_balance->size())
    {
        return QColor(0xF5F5F5);
    }
    return QVariant();
}

QVariant BalanceModel::GetCellForecolor(const QModelIndex& index) const
{
    if (static_cast<size_t>(index.row()) >= m_balance->size())
    {
        return QColor(Qt::GlobalColor::black);
    }

    const auto& balance = GetBalanceItem(index.row());

    if (index.column() == 0)
    {
        const auto account = m_accounts->at(balance.Account());
        return QColor(account->ForegroundColor());
    }
    else if (index.column() == 1)
    {
        const auto currency = m_currencies->at(balance.Currency());
        return QColor(currency->ForegroundColor());
    }

    return QVariant();
}
