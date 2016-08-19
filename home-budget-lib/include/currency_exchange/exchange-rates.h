#pragma once
#include <unordered_map>
#include "hb-types.h"

namespace hb
{
namespace currency_exchange
{
using ExchangeRates = std::unordered_map<CurrencyId, double>;

using ExchangeRatesTable = std::unordered_map<CurrencyId, ExchangeRates>;
}  // namespace currency_exchange
}  // namespace hb
