#ifndef DOCUMENTTYPE_H
#define DOCUMENTTYPE_H

#include <map>
#include <vector>
#include <memory>

#include "raw-types.h"

namespace hb
{
namespace core
{

extern const DocTypeId EmptyDocTypeId;

typedef std::vector<DocTypeId> DocTypeIdList;

class DocumentType
{
public:
    enum TypeSign
    {
        Debit = -1,
        Movement = 0,
        Credit = 1
    };

public:
    DocumentType();

    DocTypeId Id() const
    {
        return m_id;
    }

    void SetId(DocTypeId id)
    {
        m_id = id;
    }

    DocTypeId ParentId() const
    {
        return m_parentId;
    }

    void SetParentId(DocTypeId parentTypeId)
    {
        m_parentId = parentTypeId;
    }

    const DocTypeIdList& Subtypes() const
    {
        return m_subtypes;
    }

    DocTypeIdList& Subtypes()
    {
        return m_subtypes;
    }

    void AddSubtype(DocTypeId subtypeId)
    {
        m_subtypes.push_back(subtypeId);
    }

    const std::string Name() const
    {
        return m_name;
    }

    void SetName(const std::string& name)
    {
        m_name = name;
    }

    TypeSign Sign() const
    {
        return m_sign;
    }

    void SetSign(TypeSign sign)
    {
        m_sign = sign;
    }

private:

    DocTypeId      m_id;
    DocTypeId      m_parentId;

    DocTypeIdList  m_subtypes;
    std::string    m_name;

    TypeSign       m_sign;
};

typedef std::shared_ptr<DocumentType> DocumentTypePtr;


class DocumentTypeList: public std::map<DocTypeId, DocumentTypePtr>
{
public:
    void AddType(DocumentTypePtr& docType);

    const DocTypeIdList& Head() const
    {
        return m_head;
    }

    DocTypeIdList& Head()
    {
        return m_head;
    }

private:
    DocTypeIdList m_head;
};

typedef std::shared_ptr<DocumentTypeList> DocumentTypeListPtr;

void SortByName(DocTypeIdList& idList, DocumentTypeList& docTypes);

} // namespace core
} // namespace hb

#endif // DOCUMENTTYPE_H
