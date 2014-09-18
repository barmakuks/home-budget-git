#ifndef CURRENCIESMODEL_H
#define CURRENCIESMODEL_H

#include <QAbstractListModel>
#include <vector>

#include "currency.h"

class CurrenciesModel: public QAbstractListModel
{
public:
    CurrenciesModel();

    void Reload();

    hb::CurrencyId GetCurrencyItemId(int index);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;

private:
    const hb::core::Currency& GetCurrencyItem(int index) const;

    QVariant GetCellString(const QModelIndex& index) const;

private:
    std::vector<hb::core::CurrencyPtr>  m_currencies;
};

#endif // CURRENCIESMODEL_H
