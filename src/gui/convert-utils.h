#ifndef CONVERTUTILS_H
#define CONVERTUTILS_H

#include "raw-types.h"

class QDate;

namespace hb
{
namespace utils
{

QDate QDatefromNormalizedDate(const hb::Date& date);

hb::Date NormalizeDate(const QDate& date);

}
}

#endif // CONVERTUTILS_H
