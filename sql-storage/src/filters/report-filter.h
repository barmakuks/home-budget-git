#pragma once
#include <string>
#include "all-values-filter.h"

namespace hb
{
namespace storage
{
class ReportFilter : public AllValuesFilter
{
public:
    ReportFilter(Date dateMin, Date dateMax)
        : m_date_min(dateMin)
        , m_date_max(dateMax)
    {
    }

    std::string Fields() const override
    {
        return "dt.id as doc_type_id, dt.name, dt.parent_id, tr1.doc_date, tr1.cur_code, "
               "dt.operation_sign, tr1.Amount * tr1.sign as amount, short_name as cur_name, symbol "
               "as cur_symbol";
    }

    std::string WhereCondition() const override
    {
        return "operation_sign <> 0";  // + m_date_min + "' and '" + m_date_max + "'";
    }

    std::string From() const override
    {
        return "doc_types dt "
        "LEFT OUTER JOIN report tr1 ON dt.ID=tr1.doc_type_id and tr1.doc_date between '" + m_date_min + "' and '" + m_date_max + "' "
        "LEFT OUTER JOIN currency_list cl ON cl.code=tr1.cur_code";
    }

private:
    Date m_date_min;
    Date m_date_max;
};

}  // namespace storage
}  // namespace hb
