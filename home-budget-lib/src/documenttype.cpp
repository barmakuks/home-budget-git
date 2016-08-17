#include "documenttype.h"

#include <algorithm>
#include <locale>
#include <cctype>

namespace hb
{

DocumentType::DocumentType():
    m_id(EmptyId),
    m_parentId(EmptyId),
    m_sign(Direction::Movement)
{
}

std::pair<DocumentTypeList::iterator, bool> DocumentTypeList::insert(const DocumentTypeList::value_type &value)
{
    return AddType(value.second);
}

std::pair<DocumentTypeList::iterator, bool> DocumentTypeList::AddType(const DocumentTypePtr& docType)
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

    if (docType->ParentId() == EmptyId)
    {
        m_head.push_back(docType->Id());
    }

    return std::map<DocTypeId, DocumentTypePtr>::insert(value_type(docType->Id(), docType));
}

namespace
{
class CompareDocTypeByName
{
public:
    CompareDocTypeByName(const DocumentTypeList& docTypes):
        m_docTypes(docTypes)
    {
    }

    bool operator() (DocTypeId a, DocTypeId b)
    {
        return less_ci(m_docTypes.at(a)->Name(), m_docTypes.at(b)->Name());
    }

private:

    bool less_ci(const std::string& a, const std::string& b)
    {
        return a < b;  // TODO implement case insensitive comparision
    }

    const DocumentTypeList&     m_docTypes;
} ;
} // namespace

void SortByName(DocTypeIdList& idList, DocumentTypeList& docTypes)
{
    CompareDocTypeByName compare(docTypes);
    std::sort(idList.begin(), idList.end(), compare);

    for (auto id: idList)
    {
        DocumentTypePtr item = (docTypes)[id];
        SortByName(item->Subtypes(), docTypes);
    }
}

} // namespace hb
