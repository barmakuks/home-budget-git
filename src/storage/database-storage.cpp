#include "database-storage.h"

#include "idatabase-engine.h"
#include "strategies/fill-document-type-list-strategy.h"
#include "strategies/fill-documents-map-strategy.h"
#include "strategies/fill-accounts-map-strategy.h"
#include "strategies/fill-currency-map-strategy.h"
#include "strategies/last-id-strategy.h"
#include "strategies/fill-balance-map-strategy.h"
#include "strategies/fill-payments-balance-strategy.h"
#include "strategies/fill-shop-list-strategy.h"
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

    const std::string fields = filter.Fields().empty() ? std::string("*") : filter.Fields();
    const std::string where = filter.WhereCondition();
    const std::string group_by = filter.GroupByCondition();
    const std::string having = filter.HavingCondition();
    const std::string order_by = filter.OrderBy();

    const std::string query_string = "SELECT " + fields + " FROM " + filter.From()
                                     + ((where.empty()) ? std::string("") : std::string(" WHERE ") + where)
                                     + ((group_by.empty()) ? std::string("") : std::string(" GROUP BY ") + group_by)
                                     + ((having.empty()) ? std::string("") : std::string(" HAVING ") + having)
                                     + ((order_by.empty()) ? std::string("") : std::string(" ORDER BY ") + order_by);

    engine.ExecuteQuery(query_string, strategy);

    std::cout << query_string << std::endl;

    return strategy.Result();
}

template <class T>
Identifier SetLastId(IDatabaseEngine& engine, T& data)
{
    LastIdStrategy strategy;

    const std::string table = TableName(data);
    const std::string key_field = KeyField(data)->Name();

    const std::string query_string = "SELECT MAX(" + key_field + ") as LastId FROM " + table;

    engine.ExecuteQuery(query_string, strategy);

    data.SetId(strategy.Result());
    return data.Id();
}

template <>
Identifier SetLastId<Currency>(IDatabaseEngine& engine, Currency& data)
{
    data.SetId(data.Code());
    return data.Id();
}


template<typename T>
class class_has_id
{
        typedef char no;
        typedef char yes[2];
        template<class C> static yes& test(char (*)[sizeof(&C::Id)]);
        template<class C> static no& test(...);
public:
        enum{value = sizeof(test<T>(0)) == sizeof(yes&)};
};

//template <class T>
//class class_has_id
//{
//private:
//    typedef char Small;
//    struct Large
//    {
//        char a[2];
//    };

//    template <typename U, U>
//    class check
//    { };

//    template <typename C> static Small test(check<int (C::*)(), &C::Id> *);
//    template <typename C> static Large test(...);

//public:
//    enum {value = sizeof(test<T>(0)) == sizeof(Small)};
//};

template <typename T, bool readId>
class DataWriter
{
public:
    static bool Write(IDatabaseEngine& engine, T& data)
    {
        const std::string sql_query = BuildSql<T>(data);

        const bool result = engine.ExecuteNonQuery(sql_query);

        if (data.Id() <= EmptyId)
        {
            SetLastId(engine, data);
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

template <typename T>
bool DeleteData(IDatabaseEngine& engine, T& data)
{
    const std::string sql_query = BuildDeleteSql(data);
    const bool result = engine.ExecuteNonQuery(sql_query);

    return result;
}

} // namespace

DocumentTypeListPtr DatabaseStorage::GetTypeList(const IFilter& filter) const
{
    return GetData<FillDocumentTypeListStrategy>(m_databaseEngine, filter);
}

DocumentsPtr DatabaseStorage::GetDocuments(const IFilter& filter) const
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

BalancePtr DatabaseStorage::GetBalance(const IFilter& filter) const
{
    return GetData<FillBalanceMapStrategy>(m_databaseEngine, filter);
}

PaymentsBalancePtr DatabaseStorage::GetPaymentsBalance(const IFilter &filter) const
{
    return GetData<FillPaymentsBalanceStrategy>(m_databaseEngine, filter);
}

ShopListPtr DatabaseStorage::GetShopList(const IFilter& filter) const
{
    return GetData<FillShopListStrategy>(m_databaseEngine, filter);
}

bool DatabaseStorage::Write(Document& doc) const
{
    return WriteData(m_databaseEngine, doc);
}

bool DatabaseStorage::Write(DocumentType& docType) const
{
    return WriteData(m_databaseEngine, docType);
}

bool DatabaseStorage::Write(Account& account) const
{
    return WriteData(m_databaseEngine, account);
}

bool DatabaseStorage::Write(Currency& currency) const
{
    return WriteData(m_databaseEngine, currency);
}

bool DatabaseStorage::Delete(const Document& doc) const
{
    return DeleteData(m_databaseEngine, doc);
}

bool DatabaseStorage::Delete(const DocumentType& docType) const
{
    return DeleteData(m_databaseEngine, docType);
}

bool DatabaseStorage::Delete(const Account& account) const
{
    return DeleteData(m_databaseEngine, account);
}

bool DatabaseStorage::Delete(const Currency& currency) const
{
    return DeleteData(m_databaseEngine, currency);
}

} // namespace storage
} // namespace hb

