#ifndef DATAFIELDSDESCRIPTION_H
#define DATAFIELDSDESCRIPTION_H

#include "for-each-field.h"

namespace hb
{
namespace storage
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

} // namespace storage
} // namespace hb

#endif // DATAFIELDSDESCRIPTION_H