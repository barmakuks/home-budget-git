#pragma once
#include <string>
#include <sstream>

#include "documenttype.h"
#include "all-values-filter.h"

namespace hb
{
namespace sql_storage
{
class DocTypeFilter : public AllValuesFilter
{
public:
    DocTypeFilter()
    {
    }

    std::string From() const
    {
        return "doc_types";
    }
};

class DocTypeSignFilter : public DocTypeFilter
{
public:
    DocTypeSignFilter(DocumentType::Direction sign)
        : m_sign(sign)
    {
    }

    std::string WhereCondition() const
    {
        std::ostringstream where_string;
        where_string << "operation_sign = " << static_cast<int>(m_sign);

        return where_string.str();
    }

private:
    const DocumentType::Direction m_sign;
};

}  // namespace sql_storage
}  // namespace hb
