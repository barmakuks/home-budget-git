#include "payments-balance.h"

#include <vector>
#include <memory>

#include "hb-types.h"

namespace hb
{

PaymentsBalanceRow::PaymentsBalanceRow():
    m_paymentTypeId(EmptyId),
    m_amount(0)
{
}

} // namespace hb
