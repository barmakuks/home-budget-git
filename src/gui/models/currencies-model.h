#ifndef CURRENCIESMODEL_H
#define CURRENCIESMODEL_H

#include <QAbstractListModel>
#include <vector>

#include "currency.h"

class CurrenciesModel: public QAbstractListModel
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
    const hb::core::Currency& GetCurrencyItem(int index) const;

    QVariant GetCellString(const QModelIndex& index) const;

private:
    const int       m_currenciesStartIndex;

    typedef std::vector<hb::core::CurrencyPtr> CurrencyList;
    CurrencyList    m_currencies;
};

#endif // CURRENCIESMODEL_H
