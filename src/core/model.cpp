#include "model.h"
#include "assert_macro.h"

#include "doc-type-sign-filter.h"
#include "document-by-date-filter.h"
#include "all-accounts-filter.h"
#include "all-currency-filter.h"
#include "balance-filter.h"

namespace hb
{
namespace core
{
using namespace hb::storage;

std::unique_ptr<Model> Model::m_model;

Model& Model::CreateInstance(const IStoragePtr& storage)
{
    if (!m_model)
    {
        m_model.reset(new Model(storage));
    }

    return *m_model;
}

Model& Model::GetInstance()
{
    return *m_model;
}


Model::Model(const IStoragePtr& storage):
    m_storage(storage)
{
}

DocumentTypeListPtr Model::GetTypeList(DocumentType::TypeSign documentType)
{
    DocumentTypeListPtr docTypeList = m_storage->GetTypeList(DocTypeSignFilter(documentType));

    SortByName(docTypeList->Head(), *docTypeList);

    return docTypeList;
}

DocumentTypeListPtr Model::GetTypeList()
{
    return m_storage->GetTypeList(DocTypeFilter());
}

DocumentsPtr Model::GetDocuments(const Date& from,
                                 const Date& to,
                                 const AccountId accountId,
                                 const CurrencyId currencyId)
{
    return m_storage->GetDocuments(DocByDateFilter(from, to, accountId, currencyId));
}

AccountMapPtr Model::GetAccounts()
{
    return m_storage->GetAccounts(AllAccountsFilter());
}

CurrencyMapPtr Model::GetCurrencies()
{
    return m_storage->GetCurrencies(AllCurrencyFilter());
}

BalancePtr Model::GetBalance(const Date& date)
{
    return m_storage->GetBalance(BalanceFilter(date));
}

} // namespace core
} // namespace hb
