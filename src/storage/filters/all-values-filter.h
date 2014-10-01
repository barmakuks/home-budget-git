#ifndef ALLVALUESFILTER_H
#define ALLVALUESFILTER_H

#include "istorage.h"

namespace hb
{
namespace storage
{

class AllValuesFilter: public core::IFilter
{
public:
    virtual std::string Fields() const {return "*";}
    virtual std::string WhereCondition() const {return "";}
    virtual std::string GroupByCondition() const {return "";}
    virtual std::string HavingCondition() const {return "";}
    virtual std::string OrderBy() const { return "";}
};
} // namespace storage
} // namespace hb
#endif // ALLVALUESFILTER_H
