#pragma once
#include <memory>
namespace hb
{
class IFilter
{
public:
    virtual std::string Fields() const = 0;
    virtual std::string From() const = 0;
    virtual std::string WhereCondition() const = 0;
    virtual std::string GroupByCondition() const = 0;
    virtual std::string HavingCondition() const = 0;
    virtual std::string OrderBy() const = 0;
};

using IFilterPtr = std::unique_ptr<IFilter>;

}  // namespace hb
