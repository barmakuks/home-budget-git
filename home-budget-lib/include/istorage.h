#pragma once

// TODO remove include, replace with forward declaration

#include "documenttype.h"
#include "document.h"
#include "account.h"
#include "currency.h"
#include "balance.h"
#include "payments-balance.h"
//#include "payment-document.h"
#include "report-item.h"

namespace hb
{
class PaymentsPtr;
class PaymentTypesMapPtr;

class IFilter
{
public:
    virtual std::string Fields() const = 0;
    virtual std::string From() const = 0;
    virtual std::string WhereCondition() const = 0;
    virtual std::string GroupByCondition() const = 0;
    virtual std::string HavingCondition() const = 0;
    virtual std::string OrderBy() const = 0;
};

using IFilterPtr = std::unique_ptr<IFilter>;

class IFilterFactory
{
public:
    // DocTypeSignFilter
    virtual IFilterPtr CreateDocTypeFilter(hb::DocumentType::Direction doc_type_sign) const = 0;
    // DocTypeFilter
    virtual IFilterPtr CreateDocTypeFilter() const = 0;
    // DocByDateFilter
    virtual IFilterPtr CreateDocByDateFilter(const std::string& after,
                                             const std::string& before = "",
                                             const hb::AccountId accountId = hb::EmptyId,
                                             const hb::CurrencyId currencyId = hb::EmptyId,
                                             const hb::DocTypeIdList& doc_types = {}) const = 0;
    // AllAccountsFilter
    virtual IFilterPtr CreateAllAccountsFilter() const = 0;

    // AllCurrencyFilter
    virtual IFilterPtr CreateAllCurrencyFilter() const = 0;

    // BalanceFilter
    virtual IFilterPtr CreateBalanceFilter(const hb::Date& date) const = 0;

    // PaymentsBalance
    virtual IFilterPtr CreatePaymentsBalanceFilter(const hb::Date& date = "") const = 0;

    // PaymentsByDateFilter
    virtual IFilterPtr CreatePaymentsByDateFilter(const std::string& after,
                                                  const std::string& before) const = 0;

    // ShopList
    virtual IFilterPtr CreateShopListFilter() const = 0;

    // ReportFilter
    virtual IFilterPtr CreateReportFilter(const std::string& after,
                                          const std::string& before) const = 0;
};

class IStorage
{
public:
    virtual ~IStorage()
    {
    }

    /** Returns map of pairs [DocumentId, DocumentType] from storage
      @param sign
    */
    virtual DocumentTypeListPtr GetTypeList(const IFilter& filter) const = 0;

    virtual AccountMapPtr GetAccounts(const IFilter& filter) const = 0;

    virtual BalancePtr GetBalance(const IFilter& filter) const = 0;

    virtual CurrencyMapPtr GetCurrencies(const IFilter& filter) const = 0;

    virtual DocumentsPtr GetDocuments(const IFilter& filter) const = 0;

    virtual PaymentsBalancePtr GetPaymentsBalance(const IFilter& filter) const = 0;

    virtual PaymentsPtr GetPayments(const IFilter& filter) const = 0;

    virtual PaymentTypesMapPtr GetPaymentTypes(const IFilter& filter) const = 0;

    virtual ShopListPtr GetShopList(const IFilter& filter) const = 0;

    virtual ReportPtr GetReport(const IFilter& filter) const = 0;

    virtual ParamValue GetParamValue(const ParamName& paramName,
                                     const ParamValue& defaultValue = NULL) const = 0;
    virtual void SetParamValue(const ParamName& paramName, const ParamValue& value) const = 0;

    virtual bool Write(Document& doc) const = 0;
    virtual bool Write(DocumentType& docType) const = 0;
    virtual bool Write(Account& account) const = 0;
    virtual bool Write(Currency& currency) const = 0;

    virtual bool Delete(const Document& doc) const = 0;
    virtual bool Delete(const DocumentType& docType) const = 0;
    virtual bool Delete(const Account& account) const = 0;
    virtual bool Delete(const Currency& currency) const = 0;

    virtual const IFilterFactory& GetFilterFactory() const = 0;
};

typedef std::shared_ptr<IStorage> IStoragePtr;

}  // namespace hb
