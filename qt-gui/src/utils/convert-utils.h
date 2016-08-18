#pragma once
#include "hb-types.h"

class QDate;
class QString;

namespace hb
{
namespace qt_utils
{
QDate QDatefromNormalizedDate(const hb::Date& date);

hb::Date NormalizeDate(const QDate& date);

std::string Convert(const QString& source);

QString Convert(const std::string& source);

QString Tr(const std::string& source);
QString Tr(const char* source);
}
}
