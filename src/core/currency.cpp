#include "currency.h"

namespace hb
{
namespace core
{
Currency::Currency():
    m_id(EmptyId),
    m_code(EmptyId),
    m_symbol_place(false),
    m_color(0x000000)
{
}

} // namespace core
} // namespace hb
