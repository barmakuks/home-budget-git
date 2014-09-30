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
        m_maxDate(""),
        m_accountId(hb::EmptyId),
        m_currencyId(hb::EmptyId)
    {
    }

    DocByDateFilter(const std::string& after, const std::string& before):
        m_minDate(after),
        m_maxDate(before),
        m_accountId(hb::EmptyId),
        m_currencyId(hb::EmptyId)
    {
    }

    DocByDateFilter(const std::string& after,
                    const std::string& before,
                    const hb::AccountId accountId,
                    const hb::CurrencyId currencyId):
        m_minDate(after),
        m_maxDate(before),
        m_accountId(accountId),
        m_currencyId(currencyId)
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

        if (m_accountId != hb::EmptyId)
        {
            where_string << " and (account_from_id = " << m_accountId << " or account_to_id = " << m_accountId << ")";
        }

        if (m_currencyId != hb::EmptyId)
        {
            where_string << " and (account_from_cur = " << m_currencyId << " or account_to_cur = " << m_currencyId << ")";
        }

        return where_string.str();
    }

    std::string OrderBy() const
    {
        return "doc_date DESC, id";
    }

private:
    const std::string       m_minDate;
    const std::string       m_maxDate;
    const hb::AccountId     m_accountId;
    const hb::CurrencyId    m_currencyId;
};

} // namespace storage
} // namespace hb

#endif // DOCUMENTBYDATEFILTER_H
