#pragma once

#include "storage/istorage.h"

namespace hb
{
namespace sql_storage
{

class AllValuesFilter: public IFilter
{
public:
    virtual std::string Fields() const {return "*";}
    virtual std::string WhereCondition() const {return "";}
    virtual std::string GroupByCondition() const {return "";}
    virtual std::string HavingCondition() const {return "";}
    virtual std::string OrderBy() const { return "";}
};
} // namespace sql_storage
} // namespace hb
