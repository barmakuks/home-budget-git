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
typedef Identifier PaymentTypeId;

typedef unsigned int Color;

typedef int Money;
typedef std::string Date;
typedef std::string CounterValue;
typedef std::string ParamName;
typedef std::string ParamValue;

} // namespace hb

#endif // RAWTYPES_H
