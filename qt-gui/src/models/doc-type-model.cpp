#include "doc-type-model.h"
#include <stack>

#include "engine.h"
#include "get-doc-engine.h"
#include "utils/convert-utils.h"

using namespace hb;
using namespace hb::qt_utils;

DocTypeModel::DocTypeModel()
{
}

void DocTypeModel::Reload(DocumentType::Direction sign)
{
    beginResetModel();
    m_doctypes = GetDocEngine().GetTypeList(sign);
    endResetModel();
}

QModelIndex DocTypeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!m_doctypes || !hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    DocTypeIdList brothers;

    if (!parent.isValid())
    {
        brothers = m_doctypes->Head();
    }
    else
    {
        DocTypeId itemId = static_cast<DocTypeId>(parent.internalId());
        brothers = (*m_doctypes)[itemId]->Subtypes();
    }

    if (brothers.size() > static_cast<size_t>(row))
    {
        return createIndex(row, column, brothers[row]);
    }
    else
    {
        return QModelIndex();
    }
}

QModelIndex DocTypeModel::parent(const QModelIndex& child) const
{
    if (!m_doctypes || !child.isValid())
    {
        return QModelIndex();
    }

    DocTypeId childId = static_cast<DocTypeId>(child.internalId());
    DocumentTypePtr childItem = (*m_doctypes)[childId];
    DocTypeId parentId = childItem->ParentId();

    return getDocTypeIndex(parentId);
}

int DocTypeModel::rowCount(const QModelIndex& parent) const
{
    if (!m_doctypes || parent.column() > 0)
    {
        return 0;
    }

    DocTypeIdList brothers;

    if (!parent.isValid())
    {
        brothers = m_doctypes->Head();
    }
    else
    {
        brothers = (*m_doctypes)[static_cast<DocTypeId>(parent.internalId())]->Subtypes();
    }

    return brothers.size();
}

int DocTypeModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

QVariant DocTypeModel::data(const QModelIndex& index, int role) const
{
    if (!m_doctypes || !index.isValid())
    {
        return QVariant();
    }

    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }

    DocTypeId docTypeId = static_cast<DocTypeId>(index.internalId());

    return Tr((*m_doctypes)[docTypeId]->Name());
}

QModelIndex DocTypeModel::getDocTypeIndex(const DocTypeId docTypeId) const
{
    if (docTypeId == EmptyId)
    {
        return QModelIndex();
    }

    typedef std::stack<DocTypeId> DocTypeIdStack;
    DocTypeIdStack parents;

    DocTypeId id = docTypeId;

    while (id != hb::EmptyId)
    {
        parents.push(id);
        const DocumentTypePtr docType = (*m_doctypes)[id];
        id = docType->ParentId();
    }

    QModelIndex result;

    DocTypeIdList parent_brothers = m_doctypes->Head();

    do
    {
        id = parents.top();

        DocTypeIdList::const_iterator it
            = std::find(parent_brothers.begin(), parent_brothers.end(), id);

        if (it != parent_brothers.end())
        {
            int row = it - parent_brothers.begin();
            result = index(row, 0, result);
            parent_brothers = (*m_doctypes)[id]->Subtypes();
        }
        else
        {
            return QModelIndex();
        }

        parents.pop();
    } while (id != docTypeId);

    return result;
}
