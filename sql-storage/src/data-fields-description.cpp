#include "data-fields-description.h"
#include <account.h>
#include <currency.h>
#include <document.h>
#include <documenttype.h>

namespace hb
{
namespace storage
{
/***** Document type *****/

template <>
std::string TableName<hb::DocumentType>(hb::DocumentType)
{
    return "doc_types";
}

template <>
FieldDescriptionPtr KeyField<hb::DocumentType>(hb::DocumentType data)
{
    return CreateFieldDescription("id", data.Id());
}

template <>
FieldDescriptionList Fields<hb::DocumentType>(hb::DocumentType data)
{
    FieldDescriptionList list;
    list.push_back(CreateFieldDescription("parent_id", data.ParentId()));
    list.push_back(CreateFieldDescription("operation_sign", static_cast<int>(data.Sign())));
    list.push_back(CreateFieldDescription("name", data.Name()));
    return list;
}

/***** Currency *****/

template <>
std::string TableName<hb::Currency>(hb::Currency)
{
    return "currency_list";
}

template <>
FieldDescriptionPtr KeyField<hb::Currency>(hb::Currency data)
{
    return CreateFieldDescription("code", data.Code());
}

template <>
FieldDescriptionList Fields<hb::Currency>(hb::Currency data)
{
    FieldDescriptionList list;
    list.push_back(CreateFieldDescription("code", data.Code()));
    list.push_back(CreateFieldDescription("iso_code", data.IsoCode()));
    list.push_back(CreateFieldDescription("short_name", data.ShortName()));
    list.push_back(CreateFieldDescription("symbol", data.Symbol()));
    list.push_back(CreateFieldDescription("symbol_place", data.SymbolPlace()));
    list.push_back(CreateFieldDescription("fg_color", data.ForegroundColor()));
    return list;
}

/***** Account *****/
template <>
std::string TableName<hb::Account>(hb::Account)
{
    return "accounts";
}

template <>
FieldDescriptionPtr KeyField<hb::Account>(hb::Account data)
{
    return CreateFieldDescription("id", data.Id());
}

template <>
FieldDescriptionList Fields<hb::Account>(hb::Account data)
{
    FieldDescriptionList list;
    list.push_back(CreateFieldDescription("name", data.Name()));
    list.push_back(CreateFieldDescription("description", data.Description()));
    list.push_back(CreateFieldDescription("default_currency_code", data.DefaultCurrency()));
    list.push_back(CreateFieldDescription("sort_order", data.SortOrder()));
    list.push_back(CreateFieldDescription("bank", data.Bank()));
    list.push_back(CreateFieldDescription("is_active", data.IsActive()));
    list.push_back(CreateFieldDescription("foreground_color", data.ForegroundColor()));

    return list;
}

/***** Document *****/
template <>
std::string TableName<hb::Document>(hb::Document)
{
    return "documents";
}

template <>
FieldDescriptionPtr KeyField<hb::Document>(hb::Document data)
{
    return CreateFieldDescription("id", data.Id());
}

template <>
FieldDescriptionList Fields<hb::Document>(hb::Document data)
{
    hb::Amount amount;
    FieldDescriptionList list;
    list.push_back(CreateFieldDescription("doc_date", data.DocDate()));
    list.push_back(CreateFieldDescription("doc_type_id", data.DocType()));

    if (data.AmountFrom().is_initialized())
    {
        list.push_back(CreateFieldDescription("account_from_id", data.AmountFrom()->Account()));
        list.push_back(CreateFieldDescription("account_from_cur", data.AmountFrom()->Currency()));
        list.push_back(CreateFieldDescription("amount_from", data.AmountFrom()->Value()));
    }
    else
    {
        list.push_back(CreateFieldDescription("account_from_id", ""));
        list.push_back(CreateFieldDescription("account_from_cur", ""));
        list.push_back(CreateFieldDescription("amount_from", ""));
    }

    if (data.AmountTo().is_initialized())
    {
        list.push_back(CreateFieldDescription("account_to_id", data.AmountTo()->Account()));
        list.push_back(CreateFieldDescription("account_to_cur", data.AmountTo()->Currency()));
        list.push_back(CreateFieldDescription("amount_to", data.AmountTo()->Value()));
    }
    else
    {
        list.push_back(CreateFieldDescription("account_to_id", ""));
        list.push_back(CreateFieldDescription("account_to_cur", ""));
        list.push_back(CreateFieldDescription("amount_to", ""));
    }

    list.push_back(CreateFieldDescription("note", data.Note()));
    list.push_back(CreateFieldDescription("shop", data.Shop()));

    return list;
}
}
}
