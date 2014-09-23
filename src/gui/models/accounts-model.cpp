#include "accounts-model.h"

#include <QColor>
#include <QFont>

#include "engine.h"
#include "string-format.h"

AccountsModel::AccountsModel(bool allAccountsFirst):
    m_accountsStartIndex(allAccountsFirst ? 1 : 0)
{
    Reload();
}

void AccountsModel::Reload()
{
    using namespace hb::core;
    m_accounts = Engine::GetInstance().GetAccountsList(true);
}

int AccountsModel::rowCount(const QModelIndex& /*parent*/) const
{
    return m_accounts ? m_accounts->size() + m_accountsStartIndex : 0;
}

QVariant AccountsModel::data(const QModelIndex& index, int role) const
{
    switch (role)
    {
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

const hb::AccountId AccountsModel::GetAccountItemId(int index) const
{
    if (index && m_accounts && index <= m_accounts->size())
    {
        return m_accounts->at(index - m_accountsStartIndex)->Id();
    }

    return hb::EmptyId;
}

int AccountsModel::GetIndexOfAccount(hb::AccountId accountId) const
{
    using namespace hb::core;
    int index = 0;

    if (m_accounts)
    {
        for (AccountList::const_iterator it = m_accounts->begin();
             it != m_accounts->end();
             ++it, ++index)
        {
            if ((*it)->Id() == accountId)
            {
                return index;
            }
        }
    }

    return -1;
}

const hb::core::Account& AccountsModel::GetAccountItem(int index) const
{
    return *(m_accounts->at(index));
}

QVariant AccountsModel::GetCellString(const QModelIndex &index) const
{
    if (index.row() < m_accountsStartIndex)
    {
        return QObject::tr("Все счета");
    }

    return QObject::tr(hb::utils::FormatAccountName(GetAccountItem(index.row() - m_accountsStartIndex)).c_str());
}

QVariant AccountsModel::GetCellForecolor(const QModelIndex &index) const
{
    if (index.row() >= m_accountsStartIndex)
    {
        const hb::core::Account& account = GetAccountItem(index.row() - m_accountsStartIndex);

        if (account.IsActive())
        {
            return QColor(account.ForegroundColor());
        }
        else
        {
            return QColor(0xA0A0A0);
        }

    }
    return QVariant();
}

QVariant AccountsModel::GetCellBackColor(const QModelIndex &index) const
{
    return QVariant();
}

