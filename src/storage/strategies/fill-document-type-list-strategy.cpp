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
        FieldSetter<int, DocumentType, &DocumentType::SetId>::SetValue(m_currentDocType, "id", name, value);
        FieldSetter<const std::string&, DocumentType, &DocumentType::SetName>::SetValue(m_currentDocType, "name", name, value);
        FieldSetter<int, DocumentType, &DocumentType::SetParentId>::SetValue(m_currentDocType, "parent_id", name, value);
        FieldSetter<DocumentType::TypeSign, DocumentType, &DocumentType::SetSign>::SetValue(m_currentDocType, "operation_sign", name, value);
    }
}

} // namespace storage
} // namespace hb
