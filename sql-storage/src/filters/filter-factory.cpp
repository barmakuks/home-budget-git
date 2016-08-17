#include "filter-factory.h"
#include <memory>
#include "doc-type-sign-filter.h"
#include "document-by-date-filter.h"
#include "all-accounts-filter.h"
#include "all-currency-filter.h"
#include "balance-filter.h"
#include "payments-balance-filter.h"
#include "payments-filter.h"
#include "shop-filter.h"
#include "report-filter.h"
namespace hb
{
namespace storage
{
IFilterPtr FilterFactory::CreateDocTypeFilter(DocumentType::Direction doc_type_sign) const
{
    return IFilterPtr(new DocTypeSignFilter(doc_type_sign));
}

IFilterPtr FilterFactory::CreateDocTypeFilter() const
{
    return IFilterPtr(new DocTypeFilter());
}

IFilterPtr FilterFactory::CreateDocByDateFilter(const std::string& after,
                                                const std::string& before,
                                                const AccountId accountId,
                                                const CurrencyId currencyId,
                                                const DocTypeIdList& doc_types) const
{
    return IFilterPtr(new DocByDateFilter(after, before, accountId, currencyId, doc_types));
}

IFilterPtr FilterFactory::CreateAllAccountsFilter() const
{
    return IFilterPtr(new AllAccountsFilter());
}

IFilterPtr FilterFactory::CreateAllCurrencyFilter() const
{
    return IFilterPtr(new AllCurrencyFilter());
}

IFilterPtr FilterFactory::CreateBalanceFilter(const Date& date) const
{
    return IFilterPtr(new BalanceFilter(date));
}

IFilterPtr FilterFactory::CreatePaymentsBalanceFilter(const Date& date) const
{
    return IFilterPtr(new PaymentsBalanceFilter(date));
}

IFilterPtr FilterFactory::CreatePaymentsByDateFilter(const std::string& after,
                                                     const std::string& before) const
{
    return IFilterPtr(new PaymentsByDateFilter(after, before));
}

IFilterPtr FilterFactory::CreateShopListFilter() const
{
    return IFilterPtr(new ShopListFilter());
}

IFilterPtr FilterFactory::CreateReportFilter(const std::string& after,
                                             const std::string& before) const
{
    return IFilterPtr(new ReportFilter(after, before));
}

}  // namespace storage
}  // namespace hb
