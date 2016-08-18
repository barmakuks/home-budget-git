#pragma once
#include <string>
#include "all-values-filter.h"

namespace hb
{
namespace storage
{
class AllCurrencyFilter : public AllValuesFilter
{
public:
    AllCurrencyFilter()
    {
    }

    std::string From() const
    {
        return "currency_list";
    }
};

}  // namespace storage
}  // namespace hb
