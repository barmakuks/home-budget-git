#include "shops-model.h"
#include "engine.h"
#include "utils/convert-utils.h"

ShopsModel::ShopsModel()
{
}

void ShopsModel::Reload()
{
    beginResetModel();
    m_shops = hb::Engine::GetInstance().GetShops(true);
    endResetModel();
}

int ShopsModel::rowCount(const QModelIndex& /*parent*/) const
{
    return m_shops ? m_shops->size() : 0;
}

QVariant ShopsModel::data(const QModelIndex& index, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
    {
        return GetCellString(index);
    }
    default:
    {
        return QVariant();
    }
    }
}

const std::string ShopsModel::GetShopItemId(int index) const
{
    if (index < 0 || !m_shops)
    {
        return "";
    }
    return m_shops->at(index);
}

QVariant ShopsModel::GetCellString(const QModelIndex& index) const
{
    return hb::qt_utils::Tr(GetShopItemId(index.row()));
}
