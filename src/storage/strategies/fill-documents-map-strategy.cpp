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
        SetFieldValue(m_currentDoc, &Document::SetId, "id", name, value);
        SetFieldValue(m_currentDoc, &Document::SetDocDate, "doc_date", name, value);
        SetFieldValue(m_currentDoc, &Document::SetDocType, "doc_type_id", name, value);
        SetFieldValue(m_currentDoc, &Document::SetShop, "shop", name, value);
        SetFieldValue(m_currentDoc, &Document::SetNote, "note", name, value);

        SetFieldValue(m_fromAmount, &Amount::SetAccount, "account_from_id", name, value);
        SetFieldValue(m_fromAmount, &Amount::SetCurrency, "account_from_cur", name, value);
        SetFieldValue(m_fromAmount, &Amount::SetValue, "amount_from", name, value);

        SetFieldValue(m_toAmount, &Amount::SetAccount, "account_to_id", name, value);
        SetFieldValue(m_toAmount, &Amount::SetCurrency, "account_to_cur", name, value);
        SetFieldValue(m_toAmount, &Amount::SetValue, "amount_to", name, value);
    }
}


} // namespace starage
} // namespace hb
