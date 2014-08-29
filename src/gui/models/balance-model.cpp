#include "balance-model.h"
#include "all-accounts-filter.h"
#include "all-currency-filter.h"
#include "balance-filter.h"

BalanceModel::BalanceModel(hb::core::IStorage& storage,
                           const std::string& date):
    m_storage(storage)
{
    using namespace hb::storage;

    m_accounts = m_storage.GetAccounts(AllAccountsFilter());

    m_currencies = m_storage.GetCurrencies(AllCurrencyFilter());

    m_balance = m_storage.GetBalance(BalanceFilter(date));
}

QModelIndex BalanceModel::index(int row, int column, const QModelIndex &parent) const
{
    std::cout << "get index for [" << row << " : " << column << "]" << std::endl;
    return createIndex(row, column);
}

QModelIndex BalanceModel::parent(const QModelIndex &child) const
{

}

int BalanceModel::rowCount(const QModelIndex &parent) const
{
    m_balance->size();
}

int BalanceModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant BalanceModel::data(const QModelIndex &index, int role) const
{
    using namespace hb::core;

    BalanceSet::const_iterator item = m_balance->begin();

    for (size_t i = 0; i < index.row();  ++i)
    {
        item++;
    }

    BalancePtr balanceRow = *item;

    switch (index.column())
    {
    case 0:
    {
        return QVariant(m_accounts->at(balanceRow->Account())->Name().c_str());
    }
    case 1:
    {
        return balanceRow->Amount();
    }
    case 2:
    {
        return m_currencies->at(balanceRow->Currency())->ShortName().c_str();
    }
    default:
    {
        return QVariant("no data");
    }
    }
}
