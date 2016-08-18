#pragma once
#include <vector>
#include <memory>

#include "hb-types.h"

namespace hb
{

class PaymentsBalanceRow
{
public:
    PaymentsBalanceRow();

    PaymentTypeId PaymentType() const { return m_paymentTypeId; }
    void SetPaymentType(PaymentTypeId paymentTypeId) { m_paymentTypeId = paymentTypeId; }

    std::string PaymentName() const { return m_paymentName; }
    void SetPaymentName(const std::string& paymentName) { m_paymentName = paymentName; }

    Money Amount() const { return m_amount; }
    void SetAmount(Money amount) { m_amount = amount; }

private:
    PaymentTypeId   m_paymentTypeId;
    std::string     m_paymentName;
    Money           m_amount;

};

typedef std::shared_ptr<PaymentsBalanceRow> PaymentsBalanceRowPtr;

typedef std::vector<PaymentsBalanceRowPtr>  PaymentsBalance;

class PaymentsBalancePtr : public std::shared_ptr<PaymentsBalance>
{
public:
    PaymentsBalancePtr(){}
    PaymentsBalancePtr(PaymentsBalance* ptr){ reset(ptr);}
};

} // namespace hb
