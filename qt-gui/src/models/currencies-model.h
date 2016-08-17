#pragma once
#include <QAbstractListModel>
#include <vector>

#include "currency.h"

class CurrenciesModel : public QAbstractListModel
{
public:
    CurrenciesModel(bool allCurrenciesFirst = true);

    void Reload();

    hb::CurrencyId GetCurrencyItemId(int index);
    QModelIndex GetCurrencyIndex(hb::CurrencyId currencyId);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;

private:
    const hb::Currency& GetCurrencyItem(int index) const;

    QVariant GetCellString(const QModelIndex& index) const;
    QVariant GetCellForecolor(const QModelIndex& index) const;

private:
    const int m_currenciesStartIndex;

    typedef std::vector<hb::CurrencyPtr> CurrencyList;
    CurrencyList m_currencies;
};
