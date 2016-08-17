#pragma once

#include <istorage.h>

namespace hb
{
namespace storage
{
class FilterFactory : public hb::IFilterFactory
{
public:
    FilterFactory() = default;

    // IFilterFactory interface
public:
    IFilterPtr CreateDocTypeFilter(DocumentType::Direction doc_type_sign) const override;
    IFilterPtr CreateDocTypeFilter() const override;
    IFilterPtr CreateDocByDateFilter(const std::string& after,
                                     const std::string& before,
                                     const AccountId accountId,
                                     const CurrencyId currencyId,
                                     const DocTypeIdList& doc_types) const override;
    IFilterPtr CreateAllAccountsFilter() const override;
    IFilterPtr CreateAllCurrencyFilter() const override;
    IFilterPtr CreateBalanceFilter(const Date& date) const override;
    IFilterPtr CreatePaymentsBalanceFilter(const Date& date) const override;
    IFilterPtr CreatePaymentsByDateFilter(const std::string& after,
                                          const std::string& before) const override;
    IFilterPtr CreateShopListFilter() const override;
    IFilterPtr CreateReportFilter(const std::string& after,
                                  const std::string& before) const override;
};

}  // namespace storage
}  // namespace hb
