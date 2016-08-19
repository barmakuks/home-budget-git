#include "currency_exchange/currency-exchange-manager.h"

#include "currency_exchange/exchange-rates-provider.h"
#include "currency_exchange/exchange-rates-client.h"
#include "storage/exchange-rates-storage.h"
namespace hb
{
namespace currency_exchange
{
//--------------------------------------------------------------------------------------------------
class CurrencyExchangeManager::Impl : public hb::currency_exchange::ExchangeRatesClient
{
public:
    Impl(const ExchangeRatesProviderPtr& provider, const ExchangeRatesStoragePtr& storage);
    void LoadExchangeRates(const hb::Date& date);
    bool IsExchangeRatesLoaded(const hb::Date& date) const;
    ExchangeRates GetExchangeRates(const hb::Date& date, hb::CurrencyId currency) const;

    // ExchangeRatesClient interface
public:
    void OnExchangeRatesReceived(const Date& date, const ExchangeRatesTable& rates) override;

private:
    ExchangeRatesProviderPtr m_provider;
    ExchangeRatesStoragePtr m_storage;

};
//--------------------------------------------------------------------------------------------------
CurrencyExchangeManager::CurrencyExchangeManager(const ExchangeRatesProviderPtr& provider,
                                                 const ExchangeRatesStoragePtr& storage)
    : m_impl(new CurrencyExchangeManager::Impl(provider, storage))
{
}

void CurrencyExchangeManager::LoadExchangeRates(const hb::Date& date)
{
    m_impl->LoadExchangeRates(date);
}

bool CurrencyExchangeManager::IsExchangeRatesLoaded(const hb::Date& date) const
{
    return m_impl->IsExchangeRatesLoaded(date);
}

ExchangeRates CurrencyExchangeManager::GetExchangeRates(const hb::Date& date,
                                                        hb::CurrencyId currency) const
{
    return m_impl->GetExchangeRates(date, currency);
}

//--------------------------------------------------------------------------------------------------
CurrencyExchangeManager::Impl::Impl(const ExchangeRatesProviderPtr& provider,
                                    const ExchangeRatesStoragePtr& storage)
    : m_provider(provider)
    , m_storage(storage)
{
}

void CurrencyExchangeManager::Impl::LoadExchangeRates(const hb::Date& date)
{
    m_provider->RequestExchangeRates(date, this);
}

bool CurrencyExchangeManager::Impl::IsExchangeRatesLoaded(const hb::Date& date) const
{
    return m_storage->ContainsExchangeRates(date);
}

ExchangeRates CurrencyExchangeManager::Impl::GetExchangeRates(const hb::Date& date,
                                                              hb::CurrencyId currency) const
{
    return m_storage->GetExhangeRates(date, currency);
}

void CurrencyExchangeManager::Impl::OnExchangeRatesReceived(const hb::Date& date,
                                                            const ExchangeRatesTable& rates)
{
    m_storage->UpdateExchangeRates(date, rates);
}

//--------------------------------------------------------------------------------------------------
}  // namespace currency_exchange
}  // namespace hb
