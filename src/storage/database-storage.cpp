#include "database-storage.h"

#include "idatabase-engine.h"
#include "strategies/fill-document-type-list-strategy.h"
#include "strategies/fill-documents-map-strategy.h"
#include "strategies/fill-accounts-map-strategy.h"
#include "strategies/fill-currency-map-strategy.h"

namespace hb
{
namespace storage
{

using namespace hb::core;

DatabaseStorage::DatabaseStorage(IDatabaseEngine& engine):
    m_databaseEngine(engine)
{
}

namespace
{
template <class Strategy>
typename Strategy::ResultType GetData(IDatabaseEngine& engine, const IFilter& filter)
{
    Strategy strategy;

    const std::string where = filter.WhereCondition();

    const std::string query_string = "SELECT * FROM " + filter.From() + ((where.empty()) ? std::string("") : std::string(" WHERE ") + where);

    engine.ExecuteSql(query_string, strategy);

    return strategy.Result();
}
} // namespace

DocumentTypeListPtr DatabaseStorage::GetTypeList(const IFilter &filter) const
{
    return GetData<FillDocumentTypeListStrategy>(m_databaseEngine, filter);
}

DocumentsMapPtr DatabaseStorage::GetDocuments(const IFilter &filter) const
{
    return GetData<FillDocumentsMapStrategy>(m_databaseEngine, filter);
}

AccountMapPtr DatabaseStorage::GetAccounts(const core::IFilter& filter) const
{
    return GetData<FillAccountsMapStrategy>(m_databaseEngine, filter);
}

CurrencyMapPtr DatabaseStorage::GetCurrencies(const IFilter& filter) const
{
    return GetData<FillCurrencyMapStrategy>(m_databaseEngine, filter);
}


} // namespace storage
} // namespace hb
