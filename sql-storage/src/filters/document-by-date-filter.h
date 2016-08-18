#pragma once
#include <string>
#include <sstream>

#include "documenttype.h"
#include "all-values-filter.h"

namespace hb
{
namespace sql_storage
{
class DocByDateFilter : public AllValuesFilter
{
public:
    DocByDateFilter(const std::string& after)
        : m_minDate(after)
        , m_maxDate("")
        , m_accountId(hb::EmptyId)
        , m_currencyId(hb::EmptyId)
    {
    }

    DocByDateFilter(const std::string& after, const std::string& before)
        : m_minDate(after)
        , m_maxDate(before)
        , m_accountId(hb::EmptyId)
        , m_currencyId(hb::EmptyId)
    {
    }

    DocByDateFilter(const std::string& after,
                    const std::string& before,
                    const hb::AccountId accountId,
                    const hb::CurrencyId currencyId,
                    const hb::DocTypeIdList& doc_types)
        : m_minDate(after)
        , m_maxDate(before)
        , m_accountId(accountId)
        , m_currencyId(currencyId)
        , m_doc_types(doc_types)
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
            where_string << " and (account_from_id = " << m_accountId
                         << " or account_to_id = " << m_accountId << ")";
        }

        if (m_currencyId != hb::EmptyId)
        {
            where_string << " and (account_from_cur = " << m_currencyId
                         << " or account_to_cur = " << m_currencyId << ")";
        }

        if (!m_doc_types.empty())
        {
            std::ostringstream doc_types_string;

            for (DocTypeId doc_type_it : m_doc_types)
            {
                doc_types_string << doc_type_it << ", ";
            }

            const std::string str = doc_types_string.str();

            where_string << " and doc_type_id in (" << str.substr(0, str.length() - 2) << ")";
        }

        return where_string.str();
    }

    std::string OrderBy() const
    {
        return "doc_date DESC, id";
    }

private:
    const std::string m_minDate;
    const std::string m_maxDate;
    const hb::AccountId m_accountId;
    const hb::CurrencyId m_currencyId;
    const hb::DocTypeIdList m_doc_types;
};

}  // namespace sql_storage
}  // namespace hb
