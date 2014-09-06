#include "documents-model.h"

DocumentsModel::DocumentsModel()
{
}

QModelIndex DocumentsModel::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column);

}

QModelIndex DocumentsModel::parent(const QModelIndex &child) const
{

}

int DocumentsModel::rowCount(const QModelIndex &parent) const
{

}

int DocumentsModel::columnCount(const QModelIndex &parent) const
{

}

QVariant DocumentsModel::data(const QModelIndex &index, int role) const
{

}
