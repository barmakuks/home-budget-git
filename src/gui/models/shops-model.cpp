#include "shops-model.h"
#include "engine.h"
#include "convert-utils.h"

ShopsModel::ShopsModel()
{
}

void ShopsModel::Reload()
{
    using namespace hb::core;

    beginResetModel();
    m_shops = Engine::GetInstance().GetShops(true);
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
    return hb::utils::Tr(GetShopItemId(index.row()));
}
