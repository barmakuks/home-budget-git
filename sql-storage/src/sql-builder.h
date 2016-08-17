#pragma once

#include "data-fields-description.h"
#include "currency.h"
#include "documenttype.h"

namespace hb
{
namespace storage
{
std::string BuildInsertStatement(const std::string& table, const FieldDescriptionList& fields);

std::string BuildUpdateStatement(const std::string& table,
                                 const IFieldDescription& key,
                                 const FieldDescriptionList& fields);

std::string BuildDeleteStatement(const std::string& table, const IFieldDescription& key);

template <class T>
std::string BuildDeleteSql(const T& data)
{
    const std::string table = TableName(data);
    const FieldDescriptionPtr keyFieldDescription = KeyField(data);

    return BuildDeleteStatement(table, *keyFieldDescription);
}

template <>
inline std::string BuildDeleteSql<hb::DocumentType>(const hb::DocumentType& data)
{
    const FieldDescriptionPtr keyFieldDescription = KeyField(data);

    std::stringstream query_string;
    query_string
        << "UPDATE documents SET doc_type_id = (SELECT parent_id FROM doc_types WHERE id = "
        << keyFieldDescription->Value() << ") WHERE doc_type_id = " << keyFieldDescription->Value()
        << ";";
    query_string << "UPDATE doc_types SET parent_id = (SELECT parent_id FROM doc_types WHERE id = "
                 << keyFieldDescription->Value()
                 << ") WHERE parent_id = " << keyFieldDescription->Value() << ";";
    query_string << "DELETE FROM doc_types WHERE id = " << keyFieldDescription->Value() << ";";

    return query_string.str();
}

template <class T>
std::string BuildSql(const T& data)
{
    const FieldDescriptionList fieldsDescriptions = Fields(data);

    const std::string table = TableName(data);
    const FieldDescriptionPtr keyFieldDescription = KeyField(data);

    if (keyFieldDescription->Name() != "id" || keyFieldDescription->Value() == "0")
    {
        return BuildInsertStatement(table, fieldsDescriptions);
    }
    else
    {
        return BuildUpdateStatement(table, *keyFieldDescription, fieldsDescriptions);
    }
}

template <>
inline std::string BuildSql<hb::Currency>(const hb::Currency& data)
{
    const FieldDescriptionList fieldsDescriptions = Fields(data);

    const std::string table = TableName(data);
    const FieldDescriptionPtr keyFieldDescription = KeyField(data);

    if (data.Id() <= EmptyId)
    {
        return BuildInsertStatement(table, fieldsDescriptions);
    }
    else
    {
        return BuildUpdateStatement(table, *keyFieldDescription, fieldsDescriptions);
    }
}

}  // namespace storage
}  // namespace hb
