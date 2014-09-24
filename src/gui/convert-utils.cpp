#include "convert-utils.h"
#include <QDate>

namespace hb
{
namespace utils
{

QDate QDatefromNormalizedDate(const hb::Date& date)
{
    return QDate::fromString(QString::fromUtf8(date.c_str()), "yyyyMMdd");
}

hb::Date NormalizeDate(const QDate& date)
{
    return date.toString("yyyyMMdd").toUtf8().constData();
}

}
}
