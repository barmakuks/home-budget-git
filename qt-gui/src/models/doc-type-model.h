#pragma once
#include <QAbstractItemModel>
#include "documenttype.h"

class DocTypeModel : public QAbstractItemModel
{
public:
    DocTypeModel();

    void Reload(hb::DocumentType::Direction sign);
    // QAbstractItemModel interface
public:
    virtual QModelIndex index(int row, int column, const QModelIndex& parent) const;
    virtual QModelIndex parent(const QModelIndex& child) const;
    virtual int rowCount(const QModelIndex& parent) const;
    virtual int columnCount(const QModelIndex& parent) const;
    virtual QVariant data(const QModelIndex& index, int role) const;

public:
    QModelIndex getDocTypeIndex(const hb::DocTypeId docTypeId) const;

private:
    hb::DocumentTypeListPtr m_doctypes;
};
