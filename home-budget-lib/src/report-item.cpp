#include "report-item.h"

using namespace hb;

ReportItem::ReportItem():
    m_doc_type_id(EmptyId),
    m_parent_id(EmptyId)
{
}

ReportItem::ReportItem(const Report& sub_items):
    m_doc_type_id(EmptyId),
    m_parent_id(EmptyId),
    m_sub_items(sub_items)
{
    for (auto & item : sub_items)
    {
        for (auto & sub_amount: item.second->Amounts())
        {
            ReportAmountItem& amount = m_amounts[sub_amount.second.CurCode()];
            amount.SetCurCode(sub_amount.second.CurCode());
            amount.SetCurName(sub_amount.second.CurName());
            amount.SetCurSymbol(sub_amount.second.CurSymbol());
            amount.SetSubAmount(amount.SubAmount() + sub_amount.second.Amount() + sub_amount.second.SubAmount());
        }
    }
}

void ReportItem::AddAmount(const ReportAmountItem& item)
{
    auto it = m_amounts.find(item.CurCode());

    if (it != m_amounts.end())
    {
        it->second.SetAmount(it->second.Amount() + item.Amount());
    }
    else
    {
        m_amounts[item.CurCode()] = item;
    }
}

void ReportItem::AddSubAmounts(const ReportAmounts& sub_amounts)
{
    for (const auto& it : sub_amounts)
    {
        const ReportAmountItem& sub_amount = it.second;
        ReportAmountItem& amount = m_amounts[sub_amount.CurCode()];

        amount.SetSubAmount(amount.SubAmount() + sub_amount.Amount());
        amount.SetCurCode(sub_amount.CurCode());
        amount.SetCurName(sub_amount.CurName());
        amount.SetCurSymbol(sub_amount.CurSymbol());
    }
}

void ReportItem::AddSubItem(const ReportItemPtr& item)
{
    m_sub_items.insert(Report::value_type(item->Id(), item));
}

hb::ReportAmountItem::ReportAmountItem():
    m_amount(0),
    m_sub_amount(0),
    m_cur_code(EmptyId)
{
}
