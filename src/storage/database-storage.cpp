#include "database-storage.h"

#include "idatabase-engine.h"
#include "strategies/fill-document-type-list-strategy.h"
#include "strategies/fill-documents-map-strategy.h"
#include "strategies/fill-accounts-map-strategy.h"
#include "strategies/fill-currency-map-strategy.h"
#include "strategies/last-id-strategy.h"
#include "sql-builder.h"
#include "data-fields-description.h"

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

    engine.ExecuteQuery(query_string, strategy);

    return strategy.Result();
}

template <class T>
int GetLastId(IDatabaseEngine& engine)
{
    LastIdStrategy strategy;

    T data;
    const std::string table = TableName(data);
    const std::string key_field = KeyField(data)->Name();

    const std::string query_string = "SELECT MAX(" + key_field + ") as LastId FROM " + table;

    engine.ExecuteQuery(query_string, strategy);

    return strategy.Result();
}

template <class T>
class class_has_id
{
private:
    typedef char Small;
    struct Large { char a[2];};

    template <typename C> static Small test(decltype(C::Id));
    template <typename C> static Large test(...);

public:
    enum {value = sizeof(test<T>(0)) == sizeof(Small)};
};

template <typename T, bool readId>
class DataWriter
{
public:
    static bool Write(IDatabaseEngine& engine, T& data)
    {
        const std::string sql_query = BuildSql<T>(data);

        const bool result = engine.ExecuteNonQuery(sql_query);

        if (data.Id() <= 0)
        {
            data.SetId(GetLastId<T>(engine));
        }

        return result;
    }
};

template <typename T>
class DataWriter<T, false>
{
public:
    static bool Write(IDatabaseEngine& engine, T& data)
    {
        const std::string sql_query = BuildSql<T>(data);
        const bool result = engine.ExecuteNonQuery(sql_query);

        return result;
    }
};

template <typename T>
bool WriteData(IDatabaseEngine& engine, T& data)
{
    return DataWriter<T, class_has_id<T>::value>::Write(engine, data);
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

bool DatabaseStorage::Write(core::Document& doc) const
{
    return WriteData(m_databaseEngine, doc);
}

bool DatabaseStorage::Write(core::DocumentType& docType) const
{
    return WriteData(m_databaseEngine, docType);
}

bool DatabaseStorage::Write(core::Account& account) const
{
    return WriteData(m_databaseEngine, account);
}

bool DatabaseStorage::Write(core::Currency& currency) const
{
    return WriteData(m_databaseEngine, currency);
}

} // namespace storage
} // namespace hb
