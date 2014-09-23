#include "shops-model.h"
#include "engine.h"

ShopsModel::ShopsModel()
{
}

void ShopsModel::Reload()
{
    using namespace hb::core;

    m_shops = Engine::GetInstance().GetShops(true);
    reset();
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
    return m_shops->at(index);
}

QVariant ShopsModel::GetCellString(const QModelIndex& index) const
{
    return QObject::tr(GetShopItemId(index.row()).c_str());
}
