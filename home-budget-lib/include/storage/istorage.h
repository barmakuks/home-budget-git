#pragma once

// TODO remove include, replace with forward declaration

#include "documenttype.h"
#include "document.h"
#include "account.h"
#include "currency.h"
#include "balance.h"
#include "payments-balance.h"
#include "report-item.h"
#include <memory>
#include <hb-types.h>

namespace hb
{
class PaymentsPtr;
class PaymentTypesMapPtr;

class IFilter;
class IFilterFactory;

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
