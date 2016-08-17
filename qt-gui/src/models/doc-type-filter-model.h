#ifndef DOCTYPEFILTERMODEL_H
#define DOCTYPEFILTERMODEL_H

#include <QAbstractListModel>
#include "documenttype.h"

class DocTypeFilterModel: public QAbstractListModel
{
public:

    void Reload();
    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex& parent) const;
    virtual QVariant data(const QModelIndex& index, int role) const;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;

public:
    hb::DocTypeIdList GetSelected() const;
    void ResetSelection();

private:
    void  AddTypes(hb::DocTypeIdList& doctypes, const std::string& prefix = "", int level = 0);

private:
    struct CheckedDocType
    {
        CheckedDocType(hb::DocTypeId   id,
                       std::string     name,
                       bool            checked,
                       int level):
            id(id),
            name(name),
            checked(checked),
            level(level)
        {
        }
    public:
        hb::DocTypeId   id;
        std::string     name;
        bool            checked = false;
        int             level;
    };
    typedef std::vector<CheckedDocType> DocTypesNames;

    DocTypesNames m_doc_types;
};

#endif // DOCTYPEFILTERMODEL_H
