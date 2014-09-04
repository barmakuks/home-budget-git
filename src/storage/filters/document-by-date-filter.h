#ifndef DOCUMENTBYDATEFILTER_H
#define DOCUMENTBYDATEFILTER_H
#include <string>
#include <sstream>

#include "documenttype.h"
#include "all-values-filter.h"

namespace hb
{
namespace storage
{

class DocByDateFilter: public AllValuesFilter
{
public:
    DocByDateFilter(const std::string& after):
        m_minDate(after),
        m_maxDate("")
    {
    }

    DocByDateFilter(const std::string& after, const std::string& before):
        m_minDate(after),
        m_maxDate(before)
    {
    }

    std::string From() const
    {
        return "documents";
    }

    std::string WhereCondition() const
    {
        std::ostringstream where_string;

        where_string << "doc_date >= " << m_minDate;

        if (!m_maxDate.empty())
        {
            where_string << " and doc_date <= " << m_maxDate;
        }

        return where_string.str();
    }

    std::string OrderBy() const
    {
        return "doc_date, id";
    }

private:
    const std::string m_minDate;
    const std::string m_maxDate;
};

} // namespace storage
} // namespace hb

#endif // DOCUMENTBYDATEFILTER_H
