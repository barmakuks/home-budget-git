#pragma once
#include <string>

#include "documenttype.h"
#include "all-values-filter.h"

namespace hb
{
namespace sql_storage
{
class AllAccountsFilter : public AllValuesFilter
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

}  // namespace sql_storage
}  // namespace hb
