#pragma once
#include <QAbstractListModel>
#include <stack>

#include "report-item.h"

class ReportModel : public QAbstractTableModel
{
public:
    ReportModel();

    void SetReport(const hb::ReportPtr& currentReport, hb::CurrencyId currency);

    const hb::ReportItem& CurrentItemRef() const;
    void SetCurrentItem(const hb::ReportItem& item);
    bool SetCurrentItem(int index);
    const hb::ReportItemPtr GetSubItem(int index) const;

    bool Back();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;

private:
    std::string GetCellText(const QModelIndex& index) const;
    QVariant GetCellAlignment(const QModelIndex& index) const;

private:
    hb::ReportPtr m_currentReport;
    hb::ReportItem m_currentReportItem;
    hb::CurrencyId m_currentCurrency;

    std::vector<hb::ReportItemPtr> m_orderedSubItems;
    std::stack<hb::ReportItemPtr> m_backItems;
};

