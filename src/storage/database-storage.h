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

    core::DocumentTypeListPtr GetTypeList(const core::IFilter& filter) const;

    core::DocumentsMapPtr GetDocuments(const core::IFilter &filter) const;

    core::AccountMapPtr GetAccounts(const core::IFilter& filter) const;

    core::CurrencyMapPtr GetCurrencies(const core::IFilter& filter) const;

private:

    core::IDatabaseEngine&    m_databaseEngine;
};

} // namespace storage
} // namespace hb

#endif // DATABASESTORAGE_H
