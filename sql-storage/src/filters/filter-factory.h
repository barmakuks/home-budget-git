#pragma once

#include <storage/storage.h>
#include <storage/filter-factory.h>

namespace hb
{
namespace sql_storage
{
class FilterFactory : public hb::storage::FilterFactory
{
public:
    FilterFactory() = default;

    // IFilterFactory interface
public:
    storage::FilterPtr CreateDocTypeFilter(DocumentType::Direction doc_type_sign) const override;
    storage::FilterPtr CreateDocTypeFilter() const override;
    storage::FilterPtr CreateDocByDateFilter(const std::string& after,
                                             const std::string& before,
                                             const AccountId accountId,
                                             const CurrencyId currencyId,
                                             const DocTypeIdList& doc_types) const override;
    storage::FilterPtr CreateAllAccountsFilter() const override;
    storage::FilterPtr CreateAllCurrencyFilter() const override;
    storage::FilterPtr CreateBalanceFilter(const Date& date) const override;
    storage::FilterPtr CreatePaymentsBalanceFilter(const Date& date) const override;
    storage::FilterPtr CreatePaymentsByDateFilter(const std::string& after,
                                                  const std::string& before) const override;
    storage::FilterPtr CreateShopListFilter() const override;
    storage::FilterPtr CreateReportFilter(const std::string& after,
                                          const std::string& before) const override;
};

}  // namespace sql_storage
}  // namespace hb
