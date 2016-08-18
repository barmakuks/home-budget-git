#include "fill-shop-list-strategy.h"

namespace hb
{
namespace sql_storage
{

FillShopListStrategy::FillShopListStrategy():
    m_shops(new vector_type())
{
}

void FillShopListStrategy::NewRecord()
{
    m_shop.clear();
}

void FillShopListStrategy::ApplyRecord()
{
    if (!m_shop.empty())
    {
        m_shops->push_back(m_shop);
    }
}

void FillShopListStrategy::AddColumnValue(const std::string& fieldName, const std::string& value)
{
    if (fieldName == "shop")
    {
        m_shop = value;
    }
}

} // namespace sql_storage
} // namespace hb
