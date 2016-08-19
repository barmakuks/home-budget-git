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
namespace sql_storage
{
storage::FilterPtr FilterFactory::CreateDocTypeFilter(DocumentType::Direction doc_type_sign) const
{
    return storage::FilterPtr(new DocTypeSignFilter(doc_type_sign));
}

storage::FilterPtr FilterFactory::CreateDocTypeFilter() const
{
    return storage::FilterPtr(new DocTypeFilter());
}

storage::FilterPtr FilterFactory::CreateDocByDateFilter(const std::string& after,
                                                        const std::string& before,
                                                        const AccountId accountId,
                                                        const CurrencyId currencyId,
                                                        const DocTypeIdList& doc_types) const
{
    return storage::FilterPtr(new DocByDateFilter(after, before, accountId, currencyId, doc_types));
}

storage::FilterPtr FilterFactory::CreateAllAccountsFilter() const
{
    return storage::FilterPtr(new AllAccountsFilter());
}

storage::FilterPtr FilterFactory::CreateAllCurrencyFilter() const
{
    return storage::FilterPtr(new AllCurrencyFilter());
}

storage::FilterPtr FilterFactory::CreateBalanceFilter(const Date& date) const
{
    return storage::FilterPtr(new BalanceFilter(date));
}

storage::FilterPtr FilterFactory::CreatePaymentsBalanceFilter(const Date& date) const
{
    return storage::FilterPtr(new PaymentsBalanceFilter(date));
}

storage::FilterPtr FilterFactory::CreatePaymentsByDateFilter(const std::string& after,
                                                             const std::string& before) const
{
    return storage::FilterPtr(new PaymentsByDateFilter(after, before));
}

storage::FilterPtr FilterFactory::CreateShopListFilter() const
{
    return storage::FilterPtr(new ShopListFilter());
}

storage::FilterPtr FilterFactory::CreateReportFilter(const std::string& after,
                                                     const std::string& before) const
{
    return storage::FilterPtr(new ReportFilter(after, before));
}

}  // namespace sql_storage
}  // namespace hb
