#ifndef REPORTFILTER
#define REPORTFILTER

#include <string>
#include "all-values-filter.h"

namespace hb
{
namespace storage
{

class ReportFilter: public AllValuesFilter
{
public:
    ReportFilter(Date dateMin, Date dateMax):
        m_date_min(dateMin),
        m_date_max(dateMax)
    {
    }

    virtual std::string WhereCondition() const
    {
        return "doc_date between '" + m_date_min + "' and '" + m_date_max + "'";
    }

    std::string From() const
    {
        return "total_report";
    }

private:
    Date    m_date_min;
    Date    m_date_max;
};

} // namespace storage
} // namespace hb

#endif // REPORTFILTER

