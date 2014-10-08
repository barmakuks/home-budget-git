#include "fill-document-type-list-strategy.h"

#include "setters.hpp"

namespace hb
{
namespace storage
{

using namespace hb::core;

FillDocumentTypeListStrategy::FillDocumentTypeListStrategy():
    m_typelist(new DocumentTypeList())
{
}

void FillDocumentTypeListStrategy::NewRecord()
{
    m_currentDocType.reset(new DocumentType());
}

void FillDocumentTypeListStrategy::ApplyRecord()
{
    if (m_currentDocType)
    {
        m_typelist->AddType(m_currentDocType);
        m_currentDocType.reset();
    }
}

void FillDocumentTypeListStrategy::AddColumnValue(const std::string& fieldName, const std::string& value)
{
    std::string name = fieldName;
    std::transform(fieldName.begin(), fieldName.end(), name.begin(), ::tolower);

    if (m_currentDocType)
    {
        SetFieldValue(m_currentDocType, &DocumentType::SetId, "id", name, value);
        SetFieldValue(m_currentDocType, &DocumentType::SetName, "name", name, value);
        SetFieldValue(m_currentDocType, &DocumentType::SetParentId, "parent_id", name, value);
        SetFieldValue(m_currentDocType, &DocumentType::SetSign, "operation_sign", name, value);
    }
}

} // namespace storage
} // namespace hb
