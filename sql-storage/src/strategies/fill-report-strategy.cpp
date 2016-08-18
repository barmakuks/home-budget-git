#include "fill-report-strategy.h"

namespace hb
{
namespace sql_storage
{
FillReportStrategy::FillReportStrategy()
    : m_report(new Report())
{
}

void FillReportStrategy::NewRecord()
{
    m_report_item.reset(new ReportItem());
    m_amount.reset(new ReportAmountItem());
}

void FillReportStrategy::ApplyRecord()
{
    ReportAmountItem current_amount;

    auto it = m_report->find(m_report_item->Id());

    if (it != m_report->end())
    {
        it->second->SetDocTypeParent(m_report_item->DocTypeParent());
        it->second->SetDocTypeName(m_report_item->DocTypeName());
        it->second->SetDocDate(m_report_item->DocDate());
        m_report_item = it->second;
    }
    else
    {
        m_report->insert(Report::value_type(m_report_item->Id(), m_report_item));
    }

    if (m_amount)
    {
        current_amount = *m_amount;
        m_report_item->AddAmount(current_amount);
        current_amount = m_report_item->Amounts().at(m_amount->CurCode());
    }
}

void FillReportStrategy::AddColumnValue(const std::string& fieldName, const std::string& value)
{
    std::string name = fieldName;
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    SetFieldValue(m_report_item, &ReportItem::SetId, "doc_type_id", name, value);
    SetFieldValue(m_report_item, &ReportItem::SetDocTypeName, "name", name, value);
    SetFieldValue(m_report_item, &ReportItem::SetDocTypeParent, "parent_id", name, value);
    SetFieldValue(m_amount, &ReportAmountItem::SetAmount, "amount", name, value);
    SetFieldValue(m_amount, &ReportAmountItem::SetCurCode, "cur_code", name, value);
    SetFieldValue(m_amount, &ReportAmountItem::SetCurName, "cur_name", name, value);
    SetFieldValue(m_amount, &ReportAmountItem::SetCurSymbol, "cur_symbol", name, value);
}

void FillReportStrategy::Finalize()
{
    ReportPtr processed_report(new Report());

    for (auto it : *m_report)
    {
        ReportItemPtr report_item = it.second;

        if (report_item->DocTypeParent() == EmptyId)
        {
            processed_report->insert(Report::value_type(report_item->Id(), report_item));
        }

        ReportAmounts report_item_amounts = report_item->Amounts();

        while (DocTypeId parentId = report_item->DocTypeParent())  // while parentId != 0
        {
            auto it = m_report->find(parentId);

            if (it != m_report->end())
            {
                ReportItemPtr parent = it->second;
                parent->AddSubAmounts(report_item_amounts);
                parent->AddSubItem(report_item);
                report_item = parent;
            }
            else
            {
                break;
            }
        };
    }

    m_report = processed_report;
}

}  // namespace sql_storage
}  // namespace hb
