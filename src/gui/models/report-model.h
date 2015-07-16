#ifndef REPORTMODEL_H
#define REPORTMODEL_H

#include <QAbstractListModel>
#include <stack>

#include "report-item.h"

class ReportModel: public QAbstractTableModel
{
public:
    ReportModel();

    void SetReport(const hb::core::ReportPtr& currentReport, hb::CurrencyId currency);

    const hb::core::ReportItem& CurrentItemRef() const;
    void SetCurrentItem(const hb::core::ReportItem& item);
    bool SetCurrentItem(int index);
    const hb::core::ReportItemPtr GetSubItem(int index) const;

    bool Back();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex& index, int role) const;

private:

    std::string GetCellText(const QModelIndex& index) const;
    QVariant GetCellAlignment(const QModelIndex& index) const;

private:

    hb::core::ReportPtr     m_currentReport;
    hb::core::ReportItem    m_currentReportItem;
    hb::CurrencyId          m_currentCurrency;

    std::vector<hb::core::ReportItemPtr> m_orderedSubItems;
    std::stack<hb::core::ReportItemPtr>  m_backItems;
};

#endif // REPORTMODEL_H
