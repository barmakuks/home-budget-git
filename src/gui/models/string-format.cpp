#include "string-format.h"

#include <sstream>
#include <iostream>
#include <iomanip>

#include "account.h"

namespace hb
{
namespace utils
{
std::string FormatMoney(hb::Money money)
{
    std::ostringstream buf;
    buf << int(money / 100) << "." << std::setfill('0') << std::setw(2) << money % 100;

    return buf.str();
}

std::string FormatDate(const hb::Date& date)
{
    return date.substr(6, 2) + "." + date.substr(4, 2) + "." + date.substr(0, 4);
}

std::string FormatAccountName(const hb::core::Account& account)
{
    std::ostringstream buf;

    buf << account.Bank();

    if (!account.Bank().empty())
    {
        buf << " - ";
    }

    buf << account.Name();

    return buf.str();
}


}
}
