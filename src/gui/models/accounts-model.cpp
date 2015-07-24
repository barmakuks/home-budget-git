#include "accounts-model.h"

#include <QColor>
#include <QFont>

#include "engine.h"
#include "string-format.h"
#include "convert-utils.h"
using namespace hb::utils;

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
    if (index >= m_accountsStartIndex && m_accounts && static_cast<size_t>(index) <= m_accounts->size())
    {
        return m_accounts->at(index - m_accountsStartIndex)->Id();
    }

    return hb::EmptyId;
}

int AccountsModel::GetIndexOfAccount(hb::AccountId accountId) const
{
    using namespace hb::core;

    const auto it = std::find_if(m_accounts->begin(),
                                 m_accounts->end(),
                                 [accountId](AccountList::value_type it)
    {
        return (it)->Id() == accountId;
    });

    if (it != m_accounts->end())
    {
        return it - m_accounts->begin();
    }

    return -1;
}

void AccountsModel::SwapAccounts(hb::core::AccountList& accounts, int index_a, int index_b)
{
    using namespace hb::core;

    beginResetModel();

    AccountPtr account = accounts[index_a];
    AccountPtr next_account = accounts[index_b];
    const int sort_order = account->SortOrder();
    account->SetSortOrder(next_account->SortOrder());
    next_account->SetSortOrder(sort_order);
    std::swap(accounts[index_a], accounts[index_b]);
    Engine::GetInstance().Write(*accounts[index_a]);
    Engine::GetInstance().Write(*accounts[index_b]);

    endResetModel();
}

void AccountsModel::MoveAccountDown(hb::AccountId accountId)
{
    using namespace hb::core;

    int index = GetIndexOfAccount(accountId);

    if (index >= 0 && index < static_cast<int>(m_accounts->size()) - 1)
    {
        SwapAccounts(*m_accounts, index, index + 1);
    }
}

void AccountsModel::MoveAccountUp(hb::AccountId accountId)
{
    using namespace hb::core;

    int index = GetIndexOfAccount(accountId);

    if (index > 0)
    {
        SwapAccounts(*m_accounts, index, index - 1);
    }
}

const hb::core::Account& AccountsModel::GetAccountItem(int index) const
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

QVariant AccountsModel::GetCellBackColor(const QModelIndex& index) const
{
    return QVariant();
}

