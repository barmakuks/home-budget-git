#ifndef CURRENCY_H
#define CURRENCY_H

#include <map>
#include <memory>

#include "raw-types.h"

namespace hb
{
namespace core
{

class Currency
{
public:
    Currency();

    CurrencyId Id() const {return m_id;}
    void SetId(CurrencyId code) {m_id = code;}

    CurrencyId Code() const { return m_code; }
    void SetCode(CurrencyId code) { m_code = code; }

    std::string IsoCode() const { return m_iso_code; }
    void SetIsoCode(const std::string& iso_code) { m_iso_code = iso_code; }

    std::string ShortName() const { return m_short_name; }
    void SetShortName(const std::string& short_name) { m_short_name = short_name; }

    std::string Symbol() const { return m_symbol; }
    void SetSymbol(const std::string& symbol) { m_symbol = symbol; }

    bool SymbolPlace() const { return m_symbol_place; }
    void SetSymbolPlace(bool symbol_place) { m_symbol_place = symbol_place; }

    Color ForegroundColor() const {return m_color;}
    void  SetForegroundColor(Color color) {m_color = color;}

private:
    CurrencyId      m_id;
    CurrencyId      m_code;
    std::string     m_iso_code;
    std::string     m_short_name;
    std::string     m_symbol;
    bool            m_symbol_place;
    Color           m_color;
};

typedef std::shared_ptr<Currency> CurrencyPtr;

typedef std::map<CurrencyId, CurrencyPtr> CurrencyMap;
typedef std::shared_ptr<CurrencyMap> CurrencyMapPtr;

} // namespace core
} // namespace hb

#endif // CURRENCY_H
