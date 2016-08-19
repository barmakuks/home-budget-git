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

namespace storage
{
class Filter;
class FilterFactory;

class Storage
{
public:
    virtual ~Storage()
    {
    }

    /** Returns map of pairs [DocumentId, DocumentType] from storage
      @param sign
    */
    virtual DocumentTypeListPtr GetTypeList(const Filter& filter) const = 0;

    virtual AccountMapPtr GetAccounts(const Filter& filter) const = 0;

    virtual BalancePtr GetBalance(const Filter& filter) const = 0;

    virtual CurrencyMapPtr GetCurrencies(const Filter& filter) const = 0;

    virtual DocumentsPtr GetDocuments(const Filter& filter) const = 0;

    virtual PaymentsBalancePtr GetPaymentsBalance(const Filter& filter) const = 0;

    virtual PaymentsPtr GetPayments(const Filter& filter) const = 0;

    virtual PaymentTypesMapPtr GetPaymentTypes(const Filter& filter) const = 0;

    virtual ShopListPtr GetShopList(const Filter& filter) const = 0;

    virtual ReportPtr GetReport(const Filter& filter) const = 0;

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

    virtual const storage::FilterFactory& GetFilterFactory() const = 0;
};

typedef std::shared_ptr<Storage> StoragePtr;
}  // namespace storage
}  // namespace hb
