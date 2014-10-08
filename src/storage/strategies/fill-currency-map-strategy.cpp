#include "fill-currency-map-strategy.h"
#include "setters.hpp"

namespace hb
{
namespace storage
{

using namespace hb::core;

FillCurrencyMapStrategy::FillCurrencyMapStrategy():
    m_currencyMap(new CurrencyMap())
{
}

void FillCurrencyMapStrategy::NewRecord()
{
    m_currency.reset(new currency_type());
}

void FillCurrencyMapStrategy::ApplyRecord()
{
    if (m_currency)
    {
        m_currencyMap->insert(map_type::value_type(m_currency->Code(), m_currency));
    }
}

void FillCurrencyMapStrategy::AddColumnValue(const std::string& fieldName, const std::string& value)
{
    std::string name = fieldName;
    std::transform(fieldName.begin(), fieldName.end(), name.begin(), ::tolower);

    if (m_currency)
    {
        SetFieldValue(m_currency, &Currency::SetCode, "code", name, value);
        SetFieldValue(m_currency, &Currency::SetIsoCode, "iso_code", name, value);
        SetFieldValue(m_currency, &Currency::SetShortName, "short_name", name, value);
        SetFieldValue(m_currency, &Currency::SetSymbol, "symbol", name, value);
        SetFieldValue(m_currency, &Currency::SetSymbolPlace, "symbol_place", name, value);
        SetFieldValue(m_currency, &Currency::SetForegroundColor, "fg_color", name, value);
    }
}

} // namespace storage
} // namespace hb
