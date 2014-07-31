#ifndef FILLCURRENCYMAPSTRATEGY_H
#define FILLCURRENCYMAPSTRATEGY_H

#include "idatabase-engine.h"
#include "currency.h"
namespace hb
{
namespace storage
{

class FillCurrencyMapStrategy: public hb::core::IDatabaseEngine::ICallbackStrategy
{
public:
    typedef core::CurrencyMapPtr ResultType;

    FillCurrencyMapStrategy();

    virtual void NewRecord();

    virtual void ApplyRecord();

    virtual void AddColumnValue(const std::string& fieldName, const std::string& value);

    const ResultType& Result() const
    {
        return m_currencyMap;
    }

private:
    typedef ResultType::element_type    map_type;
    typedef map_type::mapped_type       record_type;
    typedef record_type::element_type   currency_type;

    ResultType  m_currencyMap;
    record_type m_currency;
};

} // namespace storage
} // namespace hb

#endif // FILLCURRENCYMAPSTRATEGY_H
