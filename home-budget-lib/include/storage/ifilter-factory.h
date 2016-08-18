#pragma once
#include <hb-types.h>
#include <documenttype.h>
#include "ifilter.h"
namespace hb
{

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

}  // namespace hb
