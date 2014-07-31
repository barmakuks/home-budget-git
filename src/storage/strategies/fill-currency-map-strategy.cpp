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
        FieldSetter<CurrencyId, Currency, &Currency::SetCode>::SetValue(m_currency, "code", name, value);
        FieldSetter<const std::string&, Currency, &Currency::SetIsoCode>::SetValue(m_currency, "iso_code", name, value);
        FieldSetter<const std::string&, Currency, &Currency::SetShortName>::SetValue(m_currency, "short_name", name, value);
        FieldSetter<const std::string&, Currency, &Currency::SetSymbol>::SetValue(m_currency, "symbol", name, value);
        FieldSetter<bool, Currency, &Currency::SetSymbolPlace>::SetValue(m_currency, "symbol_place", name, value);
    }
}

} // namespace storage
} // namespace hb
