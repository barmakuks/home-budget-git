#ifndef STRINGFORMAT_H
#define STRINGFORMAT_H

#include <string>

#include "raw-types.h"
#include "core.h"

namespace hb
{
namespace utils
{
std::string FormatMoney(hb::Money money);

std::string FormatDate(const hb::Date& date);

std::string FormatAccountName(const hb::core::Account& account);
}
}

#endif // STRINGFORMAT_H
