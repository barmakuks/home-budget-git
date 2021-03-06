#pragma once
#include "all-values-filter.h"

namespace hb
{
namespace sql_storage
{
class AllDocumentsFilter : public AllValuesFilter
{
public:
    AllDocumentsFilter()
    {
    }

    std::string From() const
    {
        return "documents";
    }
};

}  // namespace sql_storage
}  // namespace hb
