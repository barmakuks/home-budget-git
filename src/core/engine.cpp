#include "engine.h"
#include "assert_macro.h"

#include "doc-type-sign-filter.h"
#include "document-by-date-filter.h"
#include "all-accounts-filter.h"
#include "all-currency-filter.h"
#include "balance-filter.h"
#include "payments-balance-filter.h"
#include "payments-filter.h"
#include "shop-filter.h"
#include "date-time-utils.h"
#include "payment-document.h"
#include "payment-type.h"
#include <boost/lexical_cast.hpp>

namespace hb
{
namespace core
{
using namespace hb::storage;

std::unique_ptr<Engine> Engine::m_engine;

Engine& Engine::CreateInstance(const IStoragePtr& storage)
{
    if (!m_engine)
    {
        m_engine.reset(new Engine(storage));
    }

    return *m_engine;
}

Engine& Engine::GetInstance()
{
    return *m_engine;
}

DocumentPtr Engine::CreateDocument(DocumentType::TypeSign docType)
{
    hb::DocTypeId rootDocTypeId = Engine::GetInstance().GetRootDocTypeId(docType);

    if (rootDocTypeId != hb::EmptyId)
    {
        DocumentPtr doc(new hb::core::Document());
        doc->SetDocType(rootDocTypeId);

        Amount amount;
        const AccountPtr defaultAccount = *(GetAccountsList()->begin());
        amount.SetAccount(defaultAccount->Id());
        amount.SetValue(0);
        amount.SetCurrency(defaultAccount->DefaultCurrency());

        if (docType == DocumentType::Income)
        {
            doc->SetAmountTo(amount);
        }
        else if (docType == DocumentType::Outcome)
        {
            doc->SetAmountFrom(amount);
        }
        else
        {
            doc->SetAmountFrom(amount);
            doc->SetAmountTo(amount);
        }

        doc->SetDocDate(hb::utils::Now());

        return doc;
    }

    return DocumentPtr();
}


Engine::Engine(const IStoragePtr& storage):
    m_storage(storage)
{
}

DocumentTypeListPtr Engine::GetTypeList(DocumentType::TypeSign documentType)
{
    DocumentTypeListPtr docTypeList = m_storage->GetTypeList(DocTypeSignFilter(documentType));

    SortByName(docTypeList->Head(), *docTypeList);

    return docTypeList;
}

DocTypeId Engine::GetRootDocTypeId(DocumentType::TypeSign documentType)
{
    DocTypeId rootId = EmptyId;

    auto it = m_docTypeRoots.find(documentType);

    if (it == m_docTypeRoots.end())
    {
        DocumentTypeListPtr docTypeList = m_storage->GetTypeList(DocTypeSignFilter(documentType));

        if (documentType == DocumentType::Movement)
        {
            // TODO set empty default value when get param from storage will be implemented
            ParamValue paramValue = m_storage->GetParamValue("DEF_MOVEMENT", "212");

            int id = boost::lexical_cast<int>(paramValue);
            DocumentTypeList::const_iterator it = docTypeList->find(id);

            if (it != docTypeList->end())
            {
                rootId = id;
                m_docTypeRoots.insert(DocTypeRootsMap::value_type(documentType, rootId));

                return rootId;
            }
        }

        if (!docTypeList->Head().empty())
        {
            rootId = docTypeList->Head()[0];
            m_docTypeRoots.insert(DocTypeRootsMap::value_type(documentType, rootId));
        }
    }
    else
    {
        rootId = it->second;
    }

    return rootId;
}

DocumentTypeListPtr Engine::GetTypeList(bool reload)
{
    if (reload || !m_typelist)
    {
        m_typelist = m_storage->GetTypeList(DocTypeFilter());
    }

    return m_typelist;
}

DocumentsPtr Engine::GetDocuments(const Date& from,
                                  const Date& to,
                                  const AccountId accountId,
                                  const CurrencyId currencyId)
{
    return m_storage->GetDocuments(DocByDateFilter(from, to, accountId, currencyId));
}

AccountMapPtr Engine::GetAccounts(bool reload)
{
    if (reload || ! m_accounts)
    {
        m_accounts = m_storage->GetAccounts(AllAccountsFilter());
    }

    return m_accounts;
}


namespace
{
class AccountOrderComporator
{
public:
    bool operator()(const hb::core::AccountPtr& first, const hb::core::AccountPtr& second)
    {
        using namespace hb::core;

        if (first && second)
        {
            return first->SortOrder() < second->SortOrder();
        }
        else
        {
            return second != NULL;
        }
    }
};
}

AccountListPtr Engine::GetAccountsList(bool reload)
{
    if (reload || !m_accountList)
    {
        m_accountList.reset(new AccountList());
        hb::core::AccountMapPtr accounts = Engine::GetInstance().GetAccounts(reload);

        m_accountList->reserve(accounts->size());

        for (hb::core::AccountMap::const_iterator it = accounts->begin();
             it != accounts->end();
             ++it)
        {
            m_accountList->push_back(it->second);
        }

        AccountOrderComporator comporator;

        std::sort(m_accountList->begin(), m_accountList->end(), comporator);
    }

    return m_accountList;
}

CurrencyMapPtr Engine::GetCurrencies(bool reload)
{
    if (reload || !m_currencies)
    {
        m_currencies = m_storage->GetCurrencies(AllCurrencyFilter());
    }

    return m_currencies;
}

BalancePtr Engine::GetBalance(const Date& date)
{
    return m_storage->GetBalance(BalanceFilter(date));
}

PaymentsBalancePtr Engine::GetPaymentsBalance(const Date& date)
{
    return m_storage->GetPaymentsBalance(PaymentsBalanceFilter(date));
}

PaymentTypesMapPtr Engine::GetPaymentTypes(bool reload)
{
    if (reload || !m_paymentTypes)
    {
        m_paymentTypes = m_storage->GetPaymentTypes(PaymentsBalanceFilter(""));
    }

    return m_paymentTypes;
}

PaymentsPtr Engine::GetPayments(const Date& from, const Date& to)
{
    return m_storage->GetPayments(PaymentsByDateFilter(from, to));
}

ShopListPtr Engine::GetShops(bool reload)
{
    if (reload || !m_shops)
    {
        m_shops = m_storage->GetShopList(ShopListFilter());
    }

    return m_shops;
}

bool Engine::Write(Document& doc)
{
    return m_storage->Write(doc);
}

bool Engine::Write(DocumentType& docType)
{
    return m_storage->Write(docType);
}

bool Engine::Write(Account& account)
{
    return m_storage->Write(account);
}

bool Engine::Write(Currency& currency)
{
    return m_storage->Write(currency);
}

bool Engine::DeleteDocument(DocId id)
{
    Document doc;
    doc.SetId(id);

    return m_storage->Delete(doc);
}

bool Engine::DeleteDocumentType(DocTypeId id)
{
    DocumentType docType;
    docType.SetId(id);

    return m_storage->Delete(docType);
}

bool Engine::DeleteAccount(AccountId id)
{
    Account account;
    account.SetId(id);

    return m_storage->Delete(account);
}

bool Engine::DeleteCurrency(CurrencyId code)
{
    Currency currency;
    currency.SetCode(code);

    return m_storage->Delete(currency);
}

} // namespace core
} // namespace hb
