#include "currencies-model.h"

#include <QColor>

#include "engine.h"
#include "get-doc-engine.h"
#include "utils/convert-utils.h"

CurrenciesModel::CurrenciesModel(bool allCurrenciesFirst)
    : m_currenciesStartIndex(allCurrenciesFirst ? 1 : 0)
{
    Reload();
}

namespace
{
class CurrencyOrderComporator
{
public:
    bool operator()(const hb::CurrencyPtr& first, const hb::CurrencyPtr& second)
    {
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
    hb::CurrencyMapPtr currencies = hb::GetDocEngine().GetCurrencies();

    m_currencies.clear();
    m_currencies.reserve(currencies->size());

    for(auto item: *currencies)
    {
        m_currencies.push_back(item.second);
    }

    CurrencyOrderComporator comporator;

    std::sort(m_currencies.begin(), m_currencies.end(), comporator);
}

hb::CurrencyId CurrenciesModel::GetCurrencyItemId(int index)
{
    if (index >= m_currenciesStartIndex && static_cast<size_t>(index) <= m_currencies.size())
    {
        return m_currencies.at(index - m_currenciesStartIndex)->Code();
    }

    return hb::EmptyId;
}

QModelIndex CurrenciesModel::GetCurrencyIndex(hb::CurrencyId currencyId)
{
    int row = 0;

    for (CurrencyList::const_iterator it = m_currencies.begin(); it != m_currencies.end();
         ++it, ++row)
    {
        if ((*it)->Code() == currencyId)
        {
            return index(row + m_currenciesStartIndex, 0);
        }
    }

    return QModelIndex();
}

int CurrenciesModel::rowCount(const QModelIndex& /*parent*/) const
{
    return m_currencies.size() + m_currenciesStartIndex;
}

QVariant CurrenciesModel::data(const QModelIndex& index, int role) const
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
    default:
    {
        return QVariant();
    }
    }
}

const hb::Currency& CurrenciesModel::GetCurrencyItem(int index) const
{
    return *(m_currencies.at(index));
}

QVariant CurrenciesModel::GetCellString(const QModelIndex& index) const
{
    using namespace hb::qt_utils;

    if (index.row() < m_currenciesStartIndex)
    {
        return Tr("Всі валюти");
    }

    hb::Currency cur = GetCurrencyItem(index.row() - m_currenciesStartIndex);

    return Tr((cur.Symbol() + " - " + cur.ShortName()));
}

QVariant CurrenciesModel::GetCellForecolor(const QModelIndex& index) const
{
    if (index.row() >= m_currenciesStartIndex)
    {
        const hb::Currency& currency = GetCurrencyItem(index.row() - m_currenciesStartIndex);

        return QColor(currency.ForegroundColor());
    }

    return QVariant();
}
