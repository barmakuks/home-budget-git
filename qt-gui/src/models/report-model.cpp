#include "report-model.h"
#include "utils/convert-utils.h"
#include "string-format.h"

ReportModel::ReportModel()
{
}

void ReportModel::SetReport(const hb::ReportPtr& currentReport, hb::CurrencyId currency)
{
    m_currentReport = currentReport;
    m_currentCurrency = currency;

    m_backItems = std::stack<hb::ReportItemPtr>();

    if (m_currentReport)
    {
        hb::ReportItem item(*m_currentReport);
        SetCurrentItem(item);
    }
}

const hb::ReportItem& ReportModel::CurrentItemRef() const
{
    return m_currentReportItem;
}

void ReportModel::SetCurrentItem(const hb::ReportItem& item)
{
    m_currentReportItem = item;

    m_orderedSubItems.clear();
    m_orderedSubItems.reserve(item.SubItems().size());

    for (auto it : item.SubItems())
    {
        auto sub_item = it.second;

        if (sub_item->Amounts().find(m_currentCurrency) != sub_item->Amounts().end())
        {
            m_orderedSubItems.push_back(sub_item);
        }
    }

    std::sort(std::begin(m_orderedSubItems), std::end(m_orderedSubItems),
              [](const hb::ReportItemPtr& a, const hb::ReportItemPtr& b)
              {
                  return a->DocTypeName() < b->DocTypeName();
              });

    endResetModel();
}

bool ReportModel::SetCurrentItem(int index)
{
    const hb::ReportItemPtr next_item = GetSubItem(index);

    auto it = next_item->Amounts().find(m_currentCurrency);

    if (it != next_item->Amounts().end() && it->second.SubAmount() != 0)
    {
        m_backItems.push(next_item);

        SetCurrentItem(*next_item);

        return true;
    }

    return false;
}

const hb::ReportItemPtr ReportModel::GetSubItem(int index) const
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
        return hb::qt_utils::Tr(GetCellText(index));
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
        hb::Money money
            = it != item->Amounts().end() ? it->second.Amount() + it->second.SubAmount() : 0;

        return hb::qt_utils::FormatMoney(money);
    }

    case 2:
    {
        const auto it = item->Amounts().find(m_currentCurrency);

        return it != item->Amounts().end() ? hb::qt_utils::FormatMoney(it->second.Amount()) : "";
    }

    case 3:
    {
        const auto it = item->Amounts().find(m_currentCurrency);
        return it != item->Amounts().end() ? hb::qt_utils::FormatMoney(it->second.SubAmount()) : "";
    }

    default:
    {
        return "";
    }
    }
}
