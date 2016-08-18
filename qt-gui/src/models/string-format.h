#pragma once

#include <string>

#include "hb-types.h"

namespace hb
{
class Account;

namespace qt_utils
{
std::string FormatMoney(hb::Money money);

std::string FormatDate(const hb::Date& date);

std::string FormatAccountName(const hb::Account& account);
}
}

