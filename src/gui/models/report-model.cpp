#include "report-model.h"
#include "convert-utils.h"
#include "string-format.h"

ReportModel::ReportModel()
{

}

void ReportModel::SetReport(const hb::core::ReportPtr& currentReport, hb::CurrencyId currency)
{
    m_currentReport = currentReport;
    m_currentCurrency = currency;

    m_backItems = std::stack<hb::core::ReportItemPtr>();

    if (m_currentReport)
    {
        hb::core::ReportItem item(*m_currentReport);
        SetCurrentItem(item);
    }
}

const hb::core::ReportItem& ReportModel::CurrentItemRef() const
{
    return m_currentReportItem;
}

void ReportModel::SetCurrentItem(const hb::core::ReportItem& item)
{
    using namespace hb::core;

    m_currentReportItem = item;

    m_orderedSubItems.clear();
    m_orderedSubItems.reserve(item.SubItems().size());

    for (auto it : item.SubItems())
    {
        ReportItemPtr sub_item = it.second;

        if (sub_item->Amounts().find(m_currentCurrency) != sub_item->Amounts().end())
        {
            m_orderedSubItems.push_back(sub_item);
        }
    }

    std::sort(std::begin(m_orderedSubItems),
              std::end(m_orderedSubItems),
              [](const ReportItemPtr & a, const ReportItemPtr & b)
    {
        return a->DocTypeName() < b->DocTypeName();
    });

    endResetModel();
}

void ReportModel::SetCurrentItem(int index)
{
    const hb::core::ReportItemPtr next_item = GetSubItem(index);
    m_backItems.push(next_item);

    SetCurrentItem(*next_item);
}

const hb::core::ReportItemPtr ReportModel::GetSubItem(int index) const
{
    return m_orderedSubItems[index];
}

bool ReportModel::Back()
{
    if (m_backItems.empty())
    {
        return false;
    }

    m_backItems.pop();
    SetCurrentItem(m_backItems.empty() ? *m_currentReport : *m_backItems.top());

    return true;
}

int ReportModel::rowCount(const QModelIndex& parent) const
{
    return m_currentReport ? m_orderedSubItems.size() : 0;
}

int ReportModel::columnCount(const QModelIndex& parent) const
{
    return 4;
}

QVariant ReportModel::data(const QModelIndex& index, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
    {
        return hb::utils::Tr(GetCellText(index));
    }

    case Qt::TextAlignmentRole:
    {
        return GetCellAlignment(index);
    }

    default:
    {
        return QVariant();
    }
    }
}

QVariant ReportModel::GetCellAlignment(const QModelIndex& index) const
{
    if (index.column() > 0)
    {
        return Qt::AlignVCenter + Qt::AlignRight;
    }

    return Qt::AlignVCenter + Qt::AlignLeft;
}


std::string ReportModel::GetCellText(const QModelIndex& index) const
{
    const auto item = GetSubItem(index.row());

    switch (index.column())
    {
    case 0:
    {
        return item->DocTypeName();
    }

    case 1:
    {
        const auto it = item->Amounts().find(m_currentCurrency);
        hb::Money money = it != item->Amounts().end() ? it->second.Amount() + it->second.SubAmount() : 0;

        return hb::utils::FormatMoney(money);
    }

    case 2:
    {
        const auto it = item->Amounts().find(m_currentCurrency);

        return it != item->Amounts().end() ? hb::utils::FormatMoney(it->second.Amount()) : "";
    }

    case 3:
    {
        const auto it = item->Amounts().find(m_currentCurrency);
        return it != item->Amounts().end() ? hb::utils::FormatMoney(it->second.SubAmount()) : "";
    }

    default:
    {
        return "";
    }
    }
}

