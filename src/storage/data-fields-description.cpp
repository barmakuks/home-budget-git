#include "data-fields-description.h"
#include "documenttype.h"
#include "currency.h"
#include "account.h"

namespace hb
{
namespace storage
{

/***** Document type *****/

template<>
std::string TableName<hb::core::DocumentType>(hb::core::DocumentType)
{
    return "doc_types";
}

template<>
FieldDescriptionPtr KeyField<hb::core::DocumentType>(hb::core::DocumentType data)
{
    return CreateFieldDescription("id", data.Id());
}

template<>
FieldDescriptionList Fields<hb::core::DocumentType>(hb::core::DocumentType data)
{
    FieldDescriptionList list;
    list.push_back(CreateFieldDescription("parent_id", data.ParentId()));
    list.push_back(CreateFieldDescription("operation_sign", data.Sign()));
    list.push_back(CreateFieldDescription("name", data.Name()));
    return list;
}

/***** Currency *****/

template<>
std::string TableName<hb::core::Currency>(hb::core::Currency)
{
    return "currency_list";
}

template<>
FieldDescriptionPtr KeyField<hb::core::Currency>(hb::core::Currency data)
{
    return CreateFieldDescription("code", data.Code());
}

template<>
FieldDescriptionList Fields<hb::core::Currency>(hb::core::Currency data)
{
    FieldDescriptionList list;
    list.push_back(CreateFieldDescription("code", data.Code()));
    list.push_back(CreateFieldDescription("iso_code", data.IsoCode()));
    list.push_back(CreateFieldDescription("short_name", data.ShortName()));
    list.push_back(CreateFieldDescription("symbol", data.Symbol()));
    list.push_back(CreateFieldDescription("symbol_place", data.SymbolPlace()));
    return list;
}

/***** Account *****/
template<>
std::string TableName<hb::core::Account>(hb::core::Account)
{
    return "accounts";
}

template<>
FieldDescriptionPtr KeyField<hb::core::Account>(hb::core::Account data)
{
    return CreateFieldDescription("id", data.Id());
}

template<>
FieldDescriptionList Fields<hb::core::Account>(hb::core::Account data)
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
}
}

