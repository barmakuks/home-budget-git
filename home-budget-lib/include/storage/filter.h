#pragma once
#include <memory>
namespace hb
{
namespace storage
{
class Filter
{
public:
    virtual std::string Fields() const = 0;
    virtual std::string From() const = 0;
    virtual std::string WhereCondition() const = 0;
    virtual std::string GroupByCondition() const = 0;
    virtual std::string HavingCondition() const = 0;
    virtual std::string OrderBy() const = 0;
};

using FilterPtr = std::unique_ptr<Filter>;

} // namespace storage
}  // namespace hb
