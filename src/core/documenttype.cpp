#include "documenttype.h"

namespace hb
{
namespace core
{

DocumentType::DocumentType():
    m_id(EmptyId),
    m_parentId(EmptyId),
    m_sign(TypeSign::Movement)
{
}

void DocumentTypeList::AddType(DocumentTypePtr& docType)
{
    for (iterator it = begin();
         it != end();
         ++it)
    {
        DocumentTypePtr item = it->second;

        if (item->ParentId() == docType->Id())
        {
            docType->AddSubtype(item->Id());
        }
    }

    iterator parent = find(docType->ParentId());

    if (parent != end())
    {
        parent->second->AddSubtype(docType->Id());
    }

    insert(value_type(docType->Id(), docType));

    if (docType->ParentId() == EmptyId)
    {
        m_head.push_back(docType->Id());
    }
}

} // namespace core
} // namespace hb
