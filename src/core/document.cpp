#include "document.h"

namespace hb
{
namespace core
{
Document::Document():
    m_id(EmptyId),
    m_doc_type_id(EmptyId)
{
}

const DocumentPtr Document::CreateTemplate() const
{
    DocumentPtr doc(new Document());
    doc->m_id = hb::EmptyId;
    doc->m_doc_date = this->m_doc_date;
    doc->m_doc_type_id = this->m_doc_type_id;

    if (m_amount_from.is_initialized())
    {
        const Amount& this_amount = this->m_amount_from.get();
        Amount amount;
        amount.SetAccount(this_amount.Account());
        amount.SetCurrency(this_amount.Currency());
        doc->SetAmountFrom(amount);
    }
    if (m_amount_to.is_initialized())
    {
        const Amount& this_amount = this->m_amount_to.get();
        Amount amount;
        amount.SetAccount(this_amount.Account());
        amount.SetCurrency(this_amount.Currency());
        doc->SetAmountTo(amount);
    }

    doc->m_note = "";
    doc->m_shop = this->m_shop;

    return doc;
}

} // namespace core
} // namespace hb
