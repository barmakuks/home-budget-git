#ifndef DOCTYPESIGNFILTER_H
#define DOCTYPESIGNFILTER_H

#include <string>
#include <sstream>

#include "documenttype.h"

namespace hb
{
namespace storage
{

class DocTypeSignFilter: public core::IFilter
{
public:
    DocTypeSignFilter(core::DocumentType::TypeSign sign):
        m_sign(sign)
    {
    }

    std::string WhereCondition() const
    {
        std::ostringstream where_string;
        where_string << "operation_sign = " << m_sign;

        return where_string.str();
    }

    std::string From() const
    {
        return "doc_types";
    }

private:
    const core::DocumentType::TypeSign m_sign;
};

} // namespace storage
} // namespace hb

#endif // DOCTYPESIGNFILTER_H
