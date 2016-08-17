#include "database-storage.h"

#include "idatabase-engine.h"

#include "payment-document.h"
#include "payment-type.h"

#include "sql-builder.h"
#include "filters/filter-factory.h"
#include "strategies/fill-strategy.hpp"
#include "strategies/set-value-functions.h"
#include "strategies/last-id-strategy.h"
#include "strategies/fill-shop-list-strategy.h"
#include "strategies/fill-report-strategy.h"
#include <iostream>
namespace hb
{
namespace storage
{
DatabaseStorage::DatabaseStorage(IDatabaseEngine& engine)
    : m_databaseEngine(engine)
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

    const std::string query_string
        = "SELECT " + fields + " FROM " + filter.From()
          + ((where.empty()) ? std::string("") : std::string(" WHERE ") + where)
          + ((group_by.empty()) ? std::string("") : std::string(" GROUP BY ") + group_by)
          + ((having.empty()) ? std::string("") : std::string(" HAVING ") + having)
          + ((order_by.empty()) ? std::string("") : std::string(" ORDER BY ") + order_by);

    engine.ExecuteQuery(query_string, strategy);

    //    std::cout << query_string << std::endl;

    strategy.Finalize();

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

template <typename T>
class class_has_id
{
    typedef char no;
    typedef char yes[2];
    template <class C>
    static yes& test(char (*)[sizeof(&C::Id)]);
    template <class C>
    static no& test(...);

public:
    enum
    {
        value = sizeof(test<T>(0)) == sizeof(yes&)
    };
};

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

}  // namespace

DocumentTypeListPtr DatabaseStorage::GetTypeList(const IFilter& filter) const
{
    return GetData<FillStrategy<hb::DocumentTypeListPtr, SetDocTypeValue> >(m_databaseEngine,
                                                                            filter);
}

DocumentsPtr DatabaseStorage::GetDocuments(const IFilter& filter) const
{
    return GetData<FillStrategy<hb::DocumentsPtr, SetDocumentValue> >(m_databaseEngine, filter);
}

hb::AccountMapPtr DatabaseStorage::GetAccounts(const IFilter& filter) const
{
    return GetData<FillStrategy<hb::AccountMapPtr, SetAccountValue> >(m_databaseEngine, filter);
}

CurrencyMapPtr DatabaseStorage::GetCurrencies(const IFilter& filter) const
{
    return GetData<FillStrategy<hb::CurrencyMapPtr, SetCurrencyValue> >(m_databaseEngine, filter);
}

BalancePtr DatabaseStorage::GetBalance(const IFilter& filter) const
{
    return GetData<FillStrategy<hb::BalancePtr, SetBalanceValue> >(m_databaseEngine, filter);
}

PaymentsBalancePtr DatabaseStorage::GetPaymentsBalance(const IFilter& filter) const
{
    return GetData<FillStrategy<hb::PaymentsBalancePtr, SetPaymentsBalanceValue> >(m_databaseEngine,
                                                                                   filter);
}

PaymentTypesMapPtr DatabaseStorage::GetPaymentTypes(const IFilter& filter) const
{
    return GetData<FillStrategy<hb::PaymentTypesMapPtr, SetPaymentTypeValue> >(m_databaseEngine,
                                                                               filter);
}

PaymentsPtr DatabaseStorage::GetPayments(const IFilter& filter) const
{
    return GetData<FillStrategy<hb::PaymentsPtr, SetPaymentValue> >(m_databaseEngine, filter);
}

ShopListPtr DatabaseStorage::GetShopList(const IFilter& filter) const
{
    // TODO change strategy
    return GetData<FillShopListStrategy>(m_databaseEngine, filter);
}

ReportPtr DatabaseStorage::GetReport(const IFilter& filter) const
{
    return GetData<FillReportStrategy>(m_databaseEngine, filter);
}

ParamValue DatabaseStorage::GetParamValue(const ParamName& paramName,
                                          const ParamValue& defaultValue) const
{
    // TODO implement read data from params table
    return defaultValue;
}

void DatabaseStorage::SetParamValue(const ParamName& paramName, const ParamValue& value) const
{
    // TODO implement write data to params table
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

const hb::IFilterFactory& DatabaseStorage::GetFilterFactory() const
{
    static FilterFactory filter_factory;
    return filter_factory;
}

}  // namespace storage
}  // namespace hb
