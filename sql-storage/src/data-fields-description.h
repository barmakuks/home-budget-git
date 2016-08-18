#pragma once

#include "for-each-field.h"

namespace hb
{
namespace sql_storage
{

template<class T>
std::string TableName(T data);

template<class T>
FieldDescriptionPtr KeyField(T data);

template<class T>
FieldDescriptionList Fields(T data)
{
    return FieldDescriptionList();
}

} // namespace sql_storage
} // namespace hb
