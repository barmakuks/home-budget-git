#ifndef SHOPFILTER_H
#define SHOPFILTER_H

#include <string>
#include "all-values-filter.h"

namespace hb
{
namespace storage
{

class ShopListFilter: public AllValuesFilter
{
public:
    ShopListFilter():
        AllValuesFilter()
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

    std::string OrderBy() const { return "shop";}

};

} // storage
} // hb
#endif // SHOPFILTER_H
