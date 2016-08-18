#pragma once
#include <string>
#include "all-values-filter.h"

namespace hb
{
namespace sql_storage
{
class ShopListFilter : public AllValuesFilter
{
public:
    ShopListFilter()
        : AllValuesFilter()
    {
    }

    std::string Fields() const
    {
        return "DISTINCT shop";
    }

    std::string From() const
    {
        return "documents";
    }

    std::string OrderBy() const
    {
        return "shop";
    }
};

}  // namespace sql_storage
}  // namespace hb
