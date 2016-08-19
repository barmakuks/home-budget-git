#pragma once
#include <hb-types.h>
#include <documenttype.h>
#include "filter.h"
namespace hb
{
namespace storage
{
class FilterFactory
{
public:
    // DocTypeSignFilter
    virtual FilterPtr CreateDocTypeFilter(hb::DocumentType::Direction doc_type_sign) const = 0;
    // DocTypeFilter
    virtual FilterPtr CreateDocTypeFilter() const = 0;
    // DocByDateFilter
    virtual FilterPtr CreateDocByDateFilter(const std::string& after,
                                            const std::string& before = "",
                                            const hb::AccountId accountId = hb::EmptyId,
                                            const hb::CurrencyId currencyId = hb::EmptyId,
                                            const hb::DocTypeIdList& doc_types = {}) const = 0;
    // AllAccountsFilter
    virtual FilterPtr CreateAllAccountsFilter() const = 0;

    // AllCurrencyFilter
    virtual FilterPtr CreateAllCurrencyFilter() const = 0;

    // BalanceFilter
    virtual FilterPtr CreateBalanceFilter(const hb::Date& date) const = 0;

    // PaymentsBalance
    virtual FilterPtr CreatePaymentsBalanceFilter(const hb::Date& date = "") const = 0;

    // PaymentsByDateFilter
    virtual FilterPtr CreatePaymentsByDateFilter(const std::string& after,
                                                 const std::string& before) const = 0;

    // ShopList
    virtual FilterPtr CreateShopListFilter() const = 0;

    // ReportFilter
    virtual FilterPtr CreateReportFilter(const std::string& after,
                                         const std::string& before) const = 0;
};

} // namespace storage
}  // namespace hb
