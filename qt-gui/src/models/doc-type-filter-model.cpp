#include "doc-type-filter-model.h"
#include "engine.h"
#include "get-doc-engine.h"
#include "utils/convert-utils.h"

using namespace hb;

void DocTypeFilterModel::Reload()
{
    m_doc_types.clear();

    AddTypes(GetDocEngine().GetTypeList(DocumentType::Direction::Income)->Head());
    AddTypes(GetDocEngine().GetTypeList(DocumentType::Direction::Outcome)->Head());
    AddTypes(GetDocEngine().GetTypeList(DocumentType::Direction::Movement)->Head());
}

int DocTypeFilterModel::rowCount(const QModelIndex& parent) const
{
    return m_doc_types.size();
}

QVariant DocTypeFilterModel::data(const QModelIndex& index, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
    {
        return hb::qt_utils::Tr(m_doc_types[index.row()].name);
    }

    case Qt::CheckStateRole:
    {
        return m_doc_types[index.row()].checked ? Qt::Checked : Qt::Unchecked;
    }

    default:
    {
        return QVariant();
    }
    }
}

bool DocTypeFilterModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    switch (role)
    {
    case Qt::CheckStateRole:
    {
        m_doc_types[index.row()].checked = (value == Qt::Checked);

        return true;
    }

    default:
    {
        return QAbstractListModel::setData(index, value, role);
    }
    }
}

void DocTypeFilterModel::AddTypes(hb::DocTypeIdList& doctypes, const std::string& prefix, int level)
{
    DocumentTypeListPtr types = GetDocEngine().GetTypeList();

    for (hb::DocTypeId doc_type_id : doctypes)
    {
        DocumentTypePtr doc_type = (*types)[doc_type_id];

        m_doc_types.push_back(
            CheckedDocType(doc_type->Id(), prefix + doc_type->Name(), false, level));

        AddTypes(doc_type->Subtypes(), prefix + "      ", level + 1);
    }
}

Qt::ItemFlags DocTypeFilterModel::flags(const QModelIndex& index) const
{
    return QAbstractListModel::flags(index) | Qt::ItemIsUserCheckable;
}

DocTypeIdList DocTypeFilterModel::GetSelected() const
{
    DocTypeIdList result;

    for (CheckedDocType doc_type : m_doc_types)
    {
        if (doc_type.checked)
        {
            result.push_back(doc_type.id);
        }
    }

    return result;
}

void DocTypeFilterModel::ResetSelection()
{
    beginResetModel();

    for (auto& it : m_doc_types)
    {
        it.checked = false;
    }

    endResetModel();
}
