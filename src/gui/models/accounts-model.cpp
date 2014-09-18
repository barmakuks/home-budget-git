#include "accounts-model.h"

#include <QColor>
#include <QFont>

#include "model.h"
#include "string-format.h"

AccountsModel::AccountsModel()
{
    Reload();
}

namespace
{
class AccountOrderComporator
{
public:
    bool operator()(const hb::core::AccountPtr& first, const hb::core::AccountPtr& second)
    {
        using namespace hb::core;

        if (first && second)
        {
            return first->SortOrder() < second->SortOrder();
        }
        else
        {
            return second != NULL;
        }
    }
};
}

void AccountsModel::Reload()
{
    using namespace hb::core;

    hb::core::AccountMapPtr accounts = Model::GetInstance().GetAccounts();

    m_accounts.clear();
    m_accounts.reserve(accounts->size());

    foreach (auto item, *accounts)
    {
        m_accounts.push_back(item.second);
    }

    AccountOrderComporator comporator;

    std::sort(m_accounts.begin(), m_accounts.end(), comporator);
}

int AccountsModel::rowCount(const QModelIndex& /*parent*/) const
{
    return m_accounts.size() + 1;
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
    if (index && index <= m_accounts.size())
    {
        return m_accounts.at(index - 1)->Id();
    }

    return hb::EmptyId;
}

const hb::core::Account& AccountsModel::GetAccountItem(int index) const
{
    return *(m_accounts.at(index));
}

QVariant AccountsModel::GetCellString(const QModelIndex &index) const
{
    if (index.row() == 0)
    {
        return QObject::tr("Все счета");
    }

    return QObject::tr(hb::utils::FormatAccountName(GetAccountItem(index.row() - 1)).c_str());
}

QVariant AccountsModel::GetCellForecolor(const QModelIndex &index) const
{
    if (index.row() > 0)
    {
        const hb::core::Account& account = GetAccountItem(index.row() - 1);

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

