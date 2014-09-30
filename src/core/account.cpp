#include "account.h"

namespace hb
{
namespace core
{

Account::Account():
    m_id(EmptyId),
    m_defaultCurrencyId(EmptyId),
    m_sort_order(0),
    m_is_active(true),
    m_foreground_color(0)
{
}

} // namespace core
} // namespace hb
