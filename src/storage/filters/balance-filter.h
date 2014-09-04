#ifndef BALANCEFILTER_H
#define BALANCEFILTER_H

#include <string>
#include "all-values-filter.h"

namespace hb
{
namespace storage
{

class BalanceFilter: public AllValuesFilter
{
public:
    BalanceFilter(Date date):
        m_date(date)
    {
    }

    std::string Fields() const
    {
        return "account_id, account_cur, SUM(amount) as amount";
    }

    virtual std::string WhereCondition() const
    {
        return m_date.empty() ? std::string("") : "doc_date < '" + m_date + "'";
    }

    virtual std::string GroupByCondition() const
    {
        return "account_id, account_cur";
    }

    virtual std::string HavingCondition() const
    {
        return "SUM(amount) <> 0";
    }

    std::string From() const
    {
        return "balance_view";
    }

private:
    Date    m_date;
};

} // namespace storage
} // namespace hb

#endif // BALANCEFILTER_H
