#pragma once
#include "storage/istorage.h"
#include "idatabase-engine.h"

namespace hb
{
class IFilter;

namespace sql_storage
{
class DatabaseStorage : public IStorage
{
public:
    DatabaseStorage(IDatabaseEngine& engine);

    // IStorage interface
public:
    DocumentTypeListPtr GetTypeList(const IFilter& filter) const override;

    DocumentsPtr GetDocuments(const IFilter& filter) const override;

    AccountMapPtr GetAccounts(const IFilter& filter) const override;

    CurrencyMapPtr GetCurrencies(const IFilter& filter) const override;

    BalancePtr GetBalance(const IFilter& filter) const override;

    PaymentsBalancePtr GetPaymentsBalance(const IFilter& filter) const override;

    PaymentTypesMapPtr GetPaymentTypes(const IFilter& filter) const override;

    PaymentsPtr GetPayments(const IFilter& filter) const override;

    ShopListPtr GetShopList(const IFilter& filter) const override;

    ReportPtr GetReport(const IFilter& filter) const override;

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

    const IFilterFactory& GetFilterFactory() const override;

private:
    IDatabaseEngine& m_databaseEngine;
};

}  // namespace sql_storage
}  // namespace hb
