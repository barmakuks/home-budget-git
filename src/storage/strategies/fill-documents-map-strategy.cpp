#include "fill-documents-map-strategy.h"

#include <boost/lexical_cast.hpp>
#include "setters.hpp"

namespace hb
{
namespace storage
{
FillDocumentsMapStrategy::FillDocumentsMapStrategy():
    m_documents(new map_type()),
    m_fromAmount(new core::Amount()),
    m_toAmount(new core::Amount())
{
}

void FillDocumentsMapStrategy::NewRecord()
{
    m_currentDoc.reset(new doc_type());
    m_fromAmount.reset(new core::Amount());
    m_toAmount.reset(new core::Amount());
}

void FillDocumentsMapStrategy::ApplyRecord()
{
    if (m_fromAmount && m_fromAmount->Account() != 0)
    {
        m_currentDoc->SetAmountFrom(*m_fromAmount);
    }
    if (m_toAmount && m_toAmount->Account() != 0)
    {
        m_currentDoc->SetAmountTo(*m_toAmount);
    }
//    m_documents->insert(map_type::value_type(m_currentDoc->Id(), m_currentDoc));
    m_documents->push_back(m_currentDoc);
}

void FillDocumentsMapStrategy::AddColumnValue(const std::string& fieldName, const std::string& value)
{
    using namespace hb::core;

    std::string name = fieldName;
    std::transform(fieldName.begin(), fieldName.end(), name.begin(), ::tolower);

    if (m_currentDoc)
    {
        FieldSetter<DocId, Document, &Document::SetId>::SetValue(m_currentDoc, "id", name, value);
        FieldSetter<const std::string&, Document, &Document::SetDocDate>::SetValue(m_currentDoc, "doc_date", name, value);
        FieldSetter<DocTypeId, Document, &Document::SetDocType>::SetValue(m_currentDoc, "doc_type_id", name, value);
        FieldSetter<const std::string&, Document, &Document::SetShop>::SetValue(m_currentDoc, "shop", name, value);
        FieldSetter<const std::string&, Document, &Document::SetNote>::SetValue(m_currentDoc, "note", name, value);

        FieldSetter<AccountId, Amount, &Amount::SetAccount>::SetValue(m_fromAmount, "account_from_id", name, value);
        FieldSetter<CurrencyId, Amount, &Amount::SetCurrency>::SetValue(m_fromAmount, "account_from_cur", name, value);
        FieldSetter<Money, Amount, &Amount::SetValue>::SetValue(m_fromAmount, "amount_from", name, value);

        FieldSetter<AccountId, Amount, &Amount::SetAccount>::SetValue(m_toAmount, "account_to_id", name, value);
        FieldSetter<CurrencyId, Amount, &Amount::SetCurrency>::SetValue(m_toAmount, "account_to_cur", name, value);
        FieldSetter<Money, Amount, &Amount::SetValue>::SetValue(m_toAmount, "amount_to", name, value);
    }
}


} // namespace starage
} // namespace hb
