#ifndef PAYMENTSBALANCEFILTER_H
#define PAYMENTSBALANCEFILTER_H

#include <string>
#include "all-values-filter.h"

namespace hb
{
namespace storage
{

class PaymentsBalanceFilter: public AllValuesFilter
{
public:
    PaymentsBalanceFilter(Date date):
        m_date(date)
    {
    }

    std::string Fields() const
    {
        return "p.id, name, SUM(amount) as amount";
    }

    virtual std::string GroupByCondition() const
    {
        return "p.id, name";
    }

    std::string From() const
    {
        return "payments p LEFT OUTER JOIN payment_documents pd ON p.id=pd.payment_id " + (m_date.empty() ? std::string("") : " and date <= '" + m_date + "'");
    }

private:
    Date    m_date;
};

} // namespace storage
} // namespace hb

#endif // PAYMENTSBALANCEFILTER_H
