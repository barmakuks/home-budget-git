#include "documenttype.h"

#include <algorithm>
#include <locale>
#include <cctype>
//#include <boost/locale.hpp>
//#include <boost/algorithm/string.hpp>

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
//        std::string name_a = a;
//        std::string name_b = b;

//        std::transform(name_a.begin(), name_a.end(), name_a.begin(),
//                       [](unsigned char c) { return ::tolower(c); });
//        std::transform(name_b.begin(), name_b.end(), name_b.begin(),
//                       [](unsigned char c) { return ::tolower(c); });

//        return name_a < name_b;

//        boost::is_iless comp;
//        std::locale::global(boost::locale::generator().generate("ru_RU.UTF-8"));

//        return comp(a, b);
//        boost::locale::generator gen;
//        std::locale loc(gen(""));

//        return std::tolower(a, loc) < std::tolower(b, loc);
    }

    const DocumentTypeList&     m_docTypes;
} ;
} // namespace

void SortByName(DocTypeIdList& idList, const DocumentTypeList& docTypes)
{
    CompareDocTypeByName compare(docTypes);
    std::sort(idList.begin(), idList.end(), compare);
}

} // namespace core
} // namespace hb
