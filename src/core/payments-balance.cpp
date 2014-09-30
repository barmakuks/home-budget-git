#include "payments-balance.h"

#include <vector>
#include <memory>

#include "raw-types.h"

namespace hb
{
namespace core
{

PaymentsBalanceRow::PaymentsBalanceRow():
    m_paymentTypeId(EmptyId),
    m_amount(0)
{
}

} // namespace core
} // namespace hb
