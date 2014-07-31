#ifndef ALLCURRENCYFILTER_H
#define ALLCURRENCYFILTER_H

#include <string>

#include "currency.h"

namespace hb
{
namespace storage
{

class AllCurrencyFilter: public core::IFilter
{
public:
    AllCurrencyFilter()
    {
    }

    std::string WhereCondition() const
    {
        return "";
    }

    std::string From() const
    {
        return "currency_list";
    }
};

} // namespace storage
} // namespace hb

#endif // ALLCURRENCYFILTER_H
