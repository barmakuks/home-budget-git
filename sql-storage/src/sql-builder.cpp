#include "sql-builder.h"
#include "data-fields-description.h"

namespace hb
{
namespace sql_storage
{

std::string BuildInsertStatement(const std::string& table,
                                 const FieldDescriptionList& fields)
{
    std::stringstream query_string, fields_string, values_string;
    query_string << "INSERT INTO "<< table;

    fields_string << " (";
    values_string << " VALUES (";

    size_t qnty = fields.size();

    for (auto it = fields.begin();
         it != fields.end();
         ++it)
    {
        fields_string << (*it)->Name();
        values_string << (*it)->Value();

        if (--qnty)
        {
            fields_string  << ", ";
            values_string << ", ";
        }
    }

    fields_string << ")";
    values_string << ")";

    query_string << fields_string.str() << values_string.str();

    return query_string.str();
}

std::string BuildUpdateStatement(const std::string& table,
                                 const IFieldDescription& key,
                                 const FieldDescriptionList& fields)
{
    std::stringstream query_string, fields_string;
    query_string << "UPDATE "<< table << " SET ";

    size_t qnty = fields.size();

    for (auto it = fields.begin();
         it != fields.end();
         ++it)
    {
        fields_string << (*it)->Name() << "=" << (*it)->Value();

        if (--qnty)
        {
            fields_string  << ", ";
        }
    }

    query_string << fields_string.str() << " WHERE " << key.Name() << "=" << key.Value();

    return query_string.str();
}

std::string BuildDeleteStatement(const std::string& table,
                                 const IFieldDescription& key)
{
    std::stringstream query_string;
    query_string << "DELETE FROM "<< table << " WHERE " << key.Name() << "=" << key.Value();

    return query_string.str();
}

} // namespace sql_storage
} // namespace hb

