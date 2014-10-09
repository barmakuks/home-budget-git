#ifndef PAYMENTDOCUMENT_H
#define PAYMENTDOCUMENT_H

#include <memory>
#include <boost/optional.hpp>

#include "documenttype.h"
#include "raw-types.h"

namespace hb
{
namespace core
{

class PaymentDocument;

typedef std::shared_ptr<PaymentDocument> PaymentDocumentPtr;

class PaymentDocument
{
public:

    PaymentDocument();

    DocId Id() const { return m_id; }
    void SetId(DocId documentId) { m_id = documentId; }

    Date DocDate() const { return m_doc_date; }
    void SetDocDate(const Date& date) { m_doc_date = date; }

    PaymentTypeId PaymentType() const { return m_payment_type_id; }
    void SetPaymentType(const PaymentTypeId type_id) { m_payment_type_id = type_id; }

    Money Amount() const { return m_amount; }
    void SetAmount(const Money amount) { m_amount = amount; }

    std::string Note() const { return m_note; }
    void SetNote(const std::string& note) { m_note = note; }

    std::string Period() const { return m_period; }
    void SetPeriod(const std::string& period) { m_period = period; }

    CounterValue Counter1() const {return m_counter_1;}
    void SetCounter1(const CounterValue& value) { m_counter_1 = value; }

    CounterValue Counter2() const {return m_counter_2;}
    void SetCounter2(const CounterValue& value) { m_counter_2 = value; }

private:
    DocId                   m_id;
    Date                    m_doc_date;
    PaymentTypeId           m_payment_type_id;

    Money                   m_amount;

    std::string             m_note;
    std::string             m_period;
    std::string             m_counter_1;
    std::string             m_counter_2;
};

typedef std::vector<PaymentDocumentPtr> Payments;
class PaymentsPtr: public std::shared_ptr<Payments>
{
public:
    PaymentsPtr()
    {
    }

    PaymentsPtr(Payments* ptr)
    {
        reset(ptr);
    }
};

} // namespace core
} // namespace hb
#endif // PAYMENTDOCUMENT_H
