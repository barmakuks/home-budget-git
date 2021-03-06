#pragma once
#include <string>
#include <sstream>

#include "documenttype.h"
#include "all-values-filter.h"

namespace hb
{
namespace sql_storage
{
class PaymentsByDateFilter : public AllValuesFilter
{
public:
    PaymentsByDateFilter(const std::string& after, const std::string& before)
        : m_minDate(after)
        , m_maxDate(before)
    {
    }

    std::string From() const
    {
        return "payment_documents";
    }

    std::string WhereCondition() const
    {
        std::ostringstream where_string;

        where_string << "date >= " << m_minDate;

        if (!m_maxDate.empty())
        {
            where_string << " and date <= " << m_maxDate;
        }

        return where_string.str();
    }

    std::string OrderBy() const
    {
        return "date DESC, id";
    }

private:
    const std::string m_minDate;
    const std::string m_maxDate;
};
}  // namespace sql_storage
}  // namespace hb
