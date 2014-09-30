#ifndef ALLFILTER_H
#define ALLFILTER_H

#include "all-values-filter.h"

namespace hb
{
namespace storage
{

class AllDocumentsFilter: public AllValuesFilter
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

} // namespace storage
} // namespace hb

#endif // ALLFILTER_H
