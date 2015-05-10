#include "convert-utils.h"
#include <QDate>
#include <QString>
#include <QObject>

namespace hb
{
namespace utils
{

QDate QDatefromNormalizedDate(const hb::Date& date)
{
    return QDate::fromString(Convert(date), "yyyyMMdd");
}

hb::Date NormalizeDate(const QDate& date)
{
    return Convert(date.toString("yyyyMMdd"));
}

std::string Convert(const QString& source)
{
    return source.toUtf8().constData();
}

QString Convert(const std::string& source)
{
    return QString::fromUtf8(source.c_str());
}

QString Tr(const std::string& source)
{
    return QObject::tr(source.c_str());
}

QString Tr(const char* source)
{
    return QObject::tr(source);
}

}
}
