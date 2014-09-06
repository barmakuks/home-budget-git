#include "balance-model.h"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <QColor>
#include <QDate>

#include "model.h"
#include "balance.h"
#include "assert_macro.h"

BalanceModel::BalanceModel()
{
    using namespace hb::core;

    Model& model = Model::GetInstance();

    m_accounts = model.GetAccounts();

    m_currencies = model.GetCurrencies();

    Recalculate(QDate::currentDate());
}

void BalanceModel::Recalculate(const QDate& date)
{
    Recalculate(date.toString("yyyyMMdd").toUtf8().constData());
}

namespace
{

class AccountOrderComporator
{
public:
    AccountOrderComporator(const hb::core::AccountMapPtr& accounts):
        m_accounts(accounts)
    {
    }

    bool operator()(const hb::core::BalanceRowPtr& first, const hb::core::BalanceRowPtr& second)
    {
        using namespace hb::core;


        if (first->Account() == second->Account())
        {
            return first->Currency() > second->Currency();
        }
        else
        {

            const AccountPtr first_account = first->Account() != hb::EmptyId ? m_accounts->at(first->Account()) : NULL;
            const AccountPtr second_account = second->Account() != hb::EmptyId ? m_accounts->at(second->Account()) : NULL;

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
    hb::core::AccountMapPtr m_accounts;
};

}


void BalanceModel::Recalculate(const std::string& date)
{
    using namespace hb::core;
    m_balance = Model::GetInstance().GetBalance(date);
    m_total_balance = m_balance->GetTotalBalance();

    AccountOrderComporator comporator(m_accounts);

    std::sort(m_balance->begin(), m_balance->end(), comporator);
    std::sort(m_total_balance->begin(), m_total_balance->end(), comporator);

    reset();
}

QModelIndex BalanceModel::index(int row, int column, const QModelIndex& parent) const
{
    return createIndex(row, column);
}

QModelIndex BalanceModel::parent(const QModelIndex& child) const
{

}

int BalanceModel::rowCount(const QModelIndex& /*parent*/) const
{
    int row_count = (m_balance) ? m_balance->size() : 0;
    row_count += (m_total_balance) ? m_total_balance->size() : 0;
    return row_count;
}

int BalanceModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 3;
}

Qt::ItemFlags BalanceModel::flags(const QModelIndex &/*index*/) const
{
    return Qt::ItemIsEnabled;// | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable |  Qt::ItemIsEditable;
}

QVariant BalanceModel::data(const QModelIndex &index, int role) const
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

const hb::core::BalanceRow& BalanceModel::GetBalanceItem(int index) const
{
    using namespace hb::core;

    static hb::core::BalanceRow fake_balance;
    ASSERT_RETURN(m_balance, fake_balance);

    if (index < 0)
    {
        return fake_balance;
    }

    if (index >= m_balance->size())
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
    using namespace hb::core;

    const BalanceRow& prev_balance = GetBalanceItem(index.row() - 1);
    const BalanceRow& balance = GetBalanceItem(index.row());

    switch (index.column())
    {
    case 0:
    {
        std::ostringstream buf;
        hb::AccountId account = balance.Account();
        if (account != hb::EmptyId && prev_balance.Account() != account)
        {
            buf << m_accounts->at(account)->Bank();

            if (!m_accounts->at(account)->Bank().empty() )
            {
                buf << " - ";
            }

            buf << m_accounts->at(account)->Name();
        }
        else
        {
            buf << "";
        }

        return QObject::tr(buf.str().c_str());
    }
    case 1:
    {
        std::ostringstream buf;
        buf << int(balance.Amount() / 100) << "." << std::setfill('0') << std::setw(2) << balance.Amount() % 100;

        return QObject::tr(buf.str().c_str());
    }
    case 2:
    {
        std::ostringstream buf;
        buf << m_currencies->at(balance.Currency())->Symbol();

        return QObject::tr(buf.str().c_str());
    }
    default:
    {
        return QVariant("no data");
    }
    }
}

QVariant BalanceModel::GetCellAlignment(const QModelIndex& index) const
{
    if (index.column() == 1)
    {
        return Qt::AlignVCenter + Qt::AlignRight;
    }
    return QVariant();
}

QVariant BalanceModel::GetCellBackColor(const QModelIndex& index) const
{
    if (index.row() >= m_balance->size())
    {
        return QColor(0xF5F5F5);
    }
    return QVariant();
}

QVariant BalanceModel::GetCellForecolor(const QModelIndex& index) const
{
    using namespace hb::core;

    if (index.row() >= m_balance->size())
    {
        return QColor(Qt::GlobalColor::black);
    }

    const BalanceRow& balance = GetBalanceItem(index.row());

    const AccountPtr account = m_accounts->at(balance.Account());

    return QColor(account->ForegroundColor());
}

