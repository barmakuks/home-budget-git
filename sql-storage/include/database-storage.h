#pragma once
#include "storage/storage.h"
#include "storage/exchange-rates-storage.h"
#include "sql-database-engine.h"

namespace hb
{
namespace storage
{
class Filter;
}  // namespace storage

namespace sql_storage
{
class DatabaseStorage : public storage::Storage, public storage::ExchangeRatesStorage
{
public:
    DatabaseStorage(SqlDatabaseEngine& engine);

    // Storage interface
public:
    DocumentTypeListPtr GetTypeList(const storage::Filter& filter) const override;

    DocumentsPtr GetDocuments(const storage::Filter& filter) const override;

    AccountMapPtr GetAccounts(const storage::Filter& filter) const override;

    CurrencyMapPtr GetCurrencies(const storage::Filter& filter) const override;

    BalancePtr GetBalance(const storage::Filter& filter) const override;

    PaymentsBalancePtr GetPaymentsBalance(const storage::Filter& filter) const override;

    PaymentTypesMapPtr GetPaymentTypes(const storage::Filter& filter) const override;

    PaymentsPtr GetPayments(const storage::Filter& filter) const override;

    ShopListPtr GetShopList(const storage::Filter& filter) const override;

    ReportPtr GetReport(const storage::Filter& filter) const override;

    ParamValue GetParamValue(const ParamName& paramName,
                             const ParamValue& defaultValue) const override;
    void SetParamValue(const ParamName& paramName, const ParamValue& value) const override;

    bool Write(Document& doc) const override;
    bool Write(DocumentType& docType) const override;
    bool Write(Account& account) const override;
    bool Write(Currency& currency) const override;

    bool Delete(const Document& doc) const override;
    bool Delete(const DocumentType& docType) const override;
    bool Delete(const Account& account) const override;
    bool Delete(const Currency& currency) const override;

    const storage::FilterFactory& GetFilterFactory() const override;

    // ExchangeRatesStorage interface
public:
    bool ContainsExchangeRates(const Date& date) const override;
    void UpdateExchangeRates(const Date& date,
                             const currency_exchange::ExchangeRatesTable& rates) override;
    currency_exchange::ExchangeRates GetExhangeRates(const Date& date,
                                                     CurrencyId currency) const override;

private:
    SqlDatabaseEngine& m_databaseEngine;
};

}  // namespace sql_storage
}  // namespace hb
