#pragma once
#include <map>
#include "hb-types.h"

namespace hb
{
using ExchangeRate = std::map<CurrencyId, double>;

using ExchangeRateTable = std::map<CurrencyId, ExchangeRate>;
}  // namespace hb
