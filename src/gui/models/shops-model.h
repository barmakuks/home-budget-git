#ifndef SHOPSMODEL_H
#define SHOPSMODEL_H

#include <QAbstractListModel>
#include <vector>

#include "document.h"

class ShopsModel: public QAbstractListModel
{
public:
    ShopsModel();

    void Reload();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;

    const std::string GetShopItemId(int index) const;

private:

    QVariant GetCellString(const QModelIndex& index) const;

private:
    hb::core::ShopListPtr    m_shops;
};

#endif // SHOPSMODEL_H
