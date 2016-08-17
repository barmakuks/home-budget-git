#pragma once

#include <map>
#include <vector>

#include <memory>

#include "raw-types.h"

namespace hb
{

class ReportItem;

typedef std::shared_ptr<ReportItem> ReportItemPtr;

class ReportAmountItem
{
public:
    ReportAmountItem();

    /** Returns amount without sub-amounts */
    Money Amount() const
    {
        return m_amount;
    }
    void SetAmount(Money value)
    {
        m_amount = value;
    }

    /** Returns summ of all child amounts*/
    Money SubAmount() const
    {
        return m_sub_amount;
    }
    void SetSubAmount(Money value)
    {
        m_sub_amount = value;
    }

    CurrencyId CurCode() const
    {
        return m_cur_code;
    }
    void SetCurCode(CurrencyId value)
    {
        m_cur_code = value;
    }

    std::string CurName() const
    {
        return m_cur_name;
    }
    void SetCurName(const std::string& value)
    {
        m_cur_name = value;
    }

    std::string CurSymbol() const
    {
        return m_cur_symbol;
    }
    void SetCurSymbol(const std::string& value)
    {
        m_cur_symbol = value;
    }

private:

    Money                   m_amount;
    Money                   m_sub_amount;
    CurrencyId              m_cur_code;
    std::string             m_cur_name;
    std::string             m_cur_symbol;
};

typedef std::shared_ptr<ReportAmountItem> ReportAmountItemPtr;

typedef std::map<CurrencyId, ReportAmountItem> ReportAmounts;

typedef std::vector<ReportItemPtr> ReportSubItems;

typedef std::map<DocTypeId, ReportItemPtr> Report;

class ReportItem
{
public:
    ReportItem();

    ReportItem(const Report& sub_items);

    DocTypeId  Id() const
    {
        return m_doc_type_id;
    }
    void SetId(DocTypeId value)
    {
        m_doc_type_id = value;
    }

    std::string DocTypeName() const
    {
        return m_name;
    }
    void SetDocTypeName(const std::string& value)
    {
        m_name = value;
    }

    DocTypeId DocTypeParent() const
    {
        return m_parent_id;
    }
    void SetDocTypeParent(DocTypeId value)
    {
        m_parent_id = value;
    }

    Date DocDate() const
    {
        return m_doc_date;
    }
    void SetDocDate(const Date& value)
    {
        m_doc_date = value;
    }

    const ReportAmounts& Amounts() const
    {
        return m_amounts;
    }
    void AddAmount(const ReportAmountItem& item);
    void AddSubAmounts(const ReportAmounts& sub_amounts);

    const Report& SubItems() const
    {
        return m_sub_items;
    }
    void  AddSubItem(const ReportItemPtr& item);

private:
    DocTypeId       m_doc_type_id;
    std::string     m_name;
    DocTypeId       m_parent_id;
    Date            m_doc_date;
    ReportAmounts   m_amounts;
    Report          m_sub_items;
};


class ReportPtr : public std::shared_ptr<Report>
{
public:
    ReportPtr() {}
    ReportPtr(Report* ptr)
    {
        reset(ptr);
    }
};

} // namespace hb

