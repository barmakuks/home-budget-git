#include "currencies-model.h"
#include "model.h"

CurrenciesModel::CurrenciesModel()
{
    Reload();
}

namespace
{
class CurrencyOrderComporator
{
public:
    bool operator()(const hb::core::CurrencyPtr& first, const hb::core::CurrencyPtr& second)
    {
        using namespace hb::core;

        if (first && second)
        {
            return first->ShortName() < second->ShortName();
        }
        else
        {
            return second != NULL;
        }
    }
};
}

void CurrenciesModel::Reload()
{
    using namespace hb::core;

    hb::core::CurrencyMapPtr currencies = Model::GetInstance().GetCurrencies();

    m_currencies.clear();
    m_currencies.reserve(currencies->size());

    foreach (auto item, *currencies)
    {
        m_currencies.push_back(item.second);
    }

    CurrencyOrderComporator comporator;

    std::sort(m_currencies.begin(), m_currencies.end(), comporator);
}

hb::CurrencyId CurrenciesModel::GetCurrencyItemId(int index)
{
    if (index && index <= m_currencies.size())
    {
        return m_currencies.at(index - 1)->Code();
    }

    return hb::EmptyId;
}

int CurrenciesModel::rowCount(const QModelIndex& /*parent*/) const
{
    return m_currencies.size() + 1;
}

QVariant CurrenciesModel::data(const QModelIndex& index, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
    {
        return GetCellString(index);
    }
    default:
    {
        return QVariant();
    }
    }
}

const hb::core::Currency &CurrenciesModel::GetCurrencyItem(int index) const
{
    return *(m_currencies.at(index));
}

QVariant CurrenciesModel::GetCellString(const QModelIndex &index) const
{
    if (index.row() == 0)
    {
        return QObject::tr("Все валюты");
    }

    return QObject::tr(GetCurrencyItem(index.row() - 1).ShortName().c_str());
}
