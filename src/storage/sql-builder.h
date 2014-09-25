#ifndef SQLBUILDER_H
#define SQLBUILDER_H

#include "data-fields-description.h"
#include "currency.h"

namespace hb
{
namespace storage
{

std::string BuildInsertStatement(const std::string& table,
                                 const FieldDescriptionList& fields);

std::string BuildUpdateStatement(const std::string& table,
                                 const IFieldDescription& key,
                                 const FieldDescriptionList& fields);

std::string BuildDeleteStatement(const std::string& table,
                                 const IFieldDescription& key);

template <class T>
std::string BuildDeleteSql(const T& data)
{
    const std::string table = TableName(data);
    const FieldDescriptionPtr keyFieldDescription = KeyField(data);

    return BuildDeleteStatement(table, *keyFieldDescription);
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
inline std::string BuildSql<hb::core::Currency>(const hb::core::Currency& data)
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

} // namespace storage
} // namespace hb
#endif // SQLBUILDER_H
