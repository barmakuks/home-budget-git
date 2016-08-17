#ifndef ALLACCOUNTSFILTER_H
#define ALLACCOUNTSFILTER_H

#include <string>

#include "documenttype.h"
#include "all-values-filter.h"

namespace hb
{
namespace storage
{

class AllAccountsFilter: public AllValuesFilter
{
public:
    AllAccountsFilter()
    {
    }

    std::string From() const
    {
        return "accounts";
    }
};

} // namespace storage
} // namespace hb

#endif // ALLACCOUNTSFILTER_H
