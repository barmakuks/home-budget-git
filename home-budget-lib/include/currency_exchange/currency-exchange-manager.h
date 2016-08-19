#pragma once
#include <memory>
#include "exchange-rates.h"
#include "hb-types.h"

namespace hb
{
namespace storage
{
class ExchangeRatesStorage;
} // namespace storage

namespace currency_exchange
{

class ExchangeRatesProvider;
typedef std::shared_ptr<ExchangeRatesProvider> ExchangeRatesProviderPtr;

typedef std::shared_ptr<storage::ExchangeRatesStorage> ExchangeRatesStoragePtr;

class CurrencyExchangeManager
{
public:
    CurrencyExchangeManager(const ExchangeRatesProviderPtr& provider,
                            const ExchangeRatesStoragePtr& storage);

    void LoadExchangeRates(const hb::Date& date);
    bool IsExchangeRatesLoaded(const hb::Date& date) const;
    ExchangeRates GetExchangeRates(const hb::Date& date, hb::CurrencyId currency) const;
private:
    class Impl;

    std::unique_ptr<Impl> m_impl;
};

} // namespace currency_exchange
} // namespace hb

