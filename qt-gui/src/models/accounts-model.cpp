#include "accounts-model.h"

#include <QColor>
#include <QFont>

#include "engine.h"
#include "string-format.h"
#include "utils/convert-utils.h"
using namespace hb::qt_utils;

AccountsModel::AccountsModel(bool allAccountsFirst)
    : m_accountsStartIndex(allAccountsFirst ? 1 : 0)
{
    Reload();
}

void AccountsModel::Reload()
{
    m_accounts = hb::Engine::GetInstance().GetAccountsList(true);
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
    if (index >= m_accountsStartIndex && m_accounts
        && static_cast<size_t>(index) <= m_accounts->size())
    {
        return m_accounts->at(index - m_accountsStartIndex)->Id();
    }

    return hb::EmptyId;
}

int AccountsModel::GetIndexOfAccount(hb::AccountId accountId) const
{
    using namespace hb;
    int index = 0;

    if (m_accounts)
    {
        for (auto it = std::begin(*m_accounts); it != std::end(*m_accounts); ++it, ++index)
        {
            if ((*it)->Id() == accountId)
            {
                return index;
            }
        }
    }

    return -1;
}

const hb::Account& AccountsModel::GetAccountItem(int index) const
{
    return *(m_accounts->at(index));
}

QVariant AccountsModel::GetCellString(const QModelIndex& index) const
{
    if (index.row() < m_accountsStartIndex)
    {
        return Tr("Всі рахунки");
    }

    return Tr(FormatAccountName(GetAccountItem(index.row() - m_accountsStartIndex)));
}

QVariant AccountsModel::GetCellForecolor(const QModelIndex& index) const
{
    if (index.row() >= m_accountsStartIndex)
    {
        const hb::Account& account = GetAccountItem(index.row() - m_accountsStartIndex);

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

QVariant AccountsModel::GetCellBackColor(const QModelIndex& index) const
{
    return QVariant();
}
