#ifndef RAWTYPES_H
#define RAWTYPES_H

#include <string>

namespace hb
{

typedef int Identifier;

const Identifier EmptyId = 0;

typedef Identifier DocId;
typedef Identifier AccountId;
typedef Identifier CurrencyId;
typedef Identifier DocTypeId;

typedef unsigned int Color;

typedef int Money;
typedef std::string Date;

} // namespace hb

#endif // RAWTYPES_H
