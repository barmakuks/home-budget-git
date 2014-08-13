#include "last-id-strategy.h"

namespace hb
{
namespace storage
{

void LastIdStrategy::AddColumnValue(const std::string& fieldName, const std::string& value)
{
    if (fieldName == "LastId")
    {
        m_lastId = boost::lexical_cast<ResultType>(value);
    }
}

} // namespace storage
} // namespace hb
