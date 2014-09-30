#ifndef DATABASESTORAGE_H
#define DATABASESTORAGE_H

#include "istorage.h"
#include "idatabase-engine.h"

namespace hb
{
namespace core
{
class IFilter;
} // namespace core

namespace storage
{
class DatabaseStorage : public core::IStorage
{
public:
    DatabaseStorage(core::IDatabaseEngine& engine);

    // IStorage interface
public:
    virtual core::DocumentTypeListPtr GetTypeList(const core::IFilter& filter) const;

    virtual core::DocumentsPtr GetDocuments(const core::IFilter& filter) const;

    virtual core::AccountMapPtr GetAccounts(const core::IFilter& filter) const;

    virtual core::CurrencyMapPtr GetCurrencies(const core::IFilter& filter) const;

    virtual core::BalancePtr GetBalance(const core::IFilter& filter) const;

    virtual core::PaymentsBalancePtr GetPaymentsBalance(const core::IFilter &filter) const;

    virtual core::ShopListPtr GetShopList(const core::IFilter& filter) const;

    bool Write(core::Document& doc) const;
    bool Write(core::DocumentType& docType) const;
    bool Write(core::Account& account) const;
    bool Write(core::Currency& currency) const;

    bool Delete(const core::Document& doc) const;
    bool Delete(const core::DocumentType& docType) const;
    bool Delete(const core::Account& account) const;
    bool Delete(const core::Currency& currency) const;

private:

    core::IDatabaseEngine&    m_databaseEngine;

};

} // namespace storage
} // namespace hb

#endif // DATABASESTORAGE_H
