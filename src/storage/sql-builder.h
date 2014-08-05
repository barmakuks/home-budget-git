#ifndef SQLBUILDER_H
#define SQLBUILDER_H

#include "data-fields-description.h"

namespace hb
{
namespace storage
{

template <class T>
std::string BuildSql(const T& data)
{
    using namespace hb::storage;

    FieldDescriptionList fieldsDescriptions = Fields(data);

    const std::string table = TableName(data);
    FieldDescriptionPtr keyFieldDescription = KeyField(data);

    if (keyFieldDescription->Name() != "id" || keyFieldDescription->Value() == "0")
    {
        std::stringstream query, fields, values;
        query << "INSERT INTO "<< table;

        fields << " (";
        values << " VALUES (";

        size_t qnty = fieldsDescriptions.size();

        for (auto it = fieldsDescriptions.begin();
             it != fieldsDescriptions.end();
             ++it)
        {
            fields << (*it)->Name();
            values << (*it)->Value();

            if (--qnty)
            {
                fields  << ", ";
                values << ", ";
            }
        }

        fields << ")";
        values << ")";

        query << fields.str() << values.str();

        return query.str();
    }
    else
    {
        std::stringstream query, fields;
        query << "UPDATE "<< table << " SET ";

        size_t qnty = fieldsDescriptions.size();

        for (auto it = fieldsDescriptions.begin();
             it != fieldsDescriptions.end();
             ++it)
        {
            fields << (*it)->Name() << "=" << (*it)->Value();

            if (--qnty)
            {
                fields  << ", ";
            }
        }

        query << fields.str() << " WHERE " << keyFieldDescription->Name() << "=" << keyFieldDescription->Value();

        return query.str();
    }
}

} // namespace storage
} // namespace hb
#endif // SQLBUILDER_H
