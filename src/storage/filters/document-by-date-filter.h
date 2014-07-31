#ifndef DOCUMENTBYDATEFILTER_H
#define DOCUMENTBYDATEFILTER_H
#include <string>
#include <sstream>

#include "documenttype.h"

namespace hb
{
namespace storage
{

class DocByDateFilter: public core::IFilter
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

private:
    const std::string m_minDate;
    const std::string m_maxDate;
};

} // namespace storage
} // namespace hb

#endif // DOCUMENTBYDATEFILTER_H
