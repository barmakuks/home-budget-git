#ifndef ALLACCOUNTSFILTER_H
#define ALLACCOUNTSFILTER_H

#include <string>

#include "documenttype.h"

namespace hb
{
namespace storage
{

class AllAccountsFilter: public core::IFilter
{
public:
    AllAccountsFilter()
    {
    }

    std::string WhereCondition() const
    {
        return "";
    }

    std::string From() const
    {
        return "accounts";
    }
};

} // namespace storage
} // namespace hb

#endif // ALLACCOUNTSFILTER_H
