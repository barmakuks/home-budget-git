#include "engine.h"
#include "assert_macro.h"

#include "hb_chrono.h"
#include <memory>
#include "payment-document.h"
#include "payment-type.h"
#include <boost/lexical_cast.hpp>
#include <istorage.h>

namespace hb
{
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

DocumentPtr Engine::CreateDocument(DocumentType::Direction docType)
{
    hb::DocTypeId rootDocTypeId = Engine::GetInstance().GetRootDocTypeId(docType);

    if (rootDocTypeId != hb::EmptyId)
    {
        DocumentPtr doc(new Document());
        doc->SetDocType(rootDocTypeId);

        Amount amount;
        const AccountPtr defaultAccount = *(GetAccountsList()->begin());
        amount.SetAccount(defaultAccount->Id());
        amount.SetValue(0);
        amount.SetCurrency(defaultAccount->DefaultCurrency());

        if (docType == DocumentType::Direction::Income)
        {
            doc->SetAmountTo(amount);
        }
        else if (docType == DocumentType::Direction::Outcome)
        {
            doc->SetAmountFrom(amount);
        }
        else
        {
            doc->SetAmountFrom(amount);
            doc->SetAmountTo(amount);
        }

        doc->SetDocDate(hb::chrono::Now());

        return doc;
    }

    return DocumentPtr();
}

Engine::Engine(const IStoragePtr& storage)
    : m_storage(storage)
{
}

DocumentTypeListPtr Engine::GetTypeList(DocumentType::Direction documentType)
{
    const auto filter = m_storage->GetFilterFactory().CreateDocTypeFilter(documentType);
    DocumentTypeListPtr docTypeList = m_storage->GetTypeList(*filter);

    SortByName(docTypeList->Head(), *docTypeList);

    return docTypeList;
}

DocTypeId Engine::GetRootDocTypeId(DocumentType::Direction documentType)
{
    DocTypeId rootId = EmptyId;

    auto it = m_docTypeRoots.find(documentType);

    if (it == m_docTypeRoots.end())
    {
        const auto filter = m_storage->GetFilterFactory().CreateDocTypeFilter(documentType);
        DocumentTypeListPtr docTypeList = m_storage->GetTypeList(*filter);

        if (documentType == DocumentType::Direction::Movement)
        {
            // TODO set empty default value when get param from storage will be
            // implemented
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
        static const auto filter = m_storage->GetFilterFactory().CreateDocTypeFilter();
        m_typelist = m_storage->GetTypeList(*filter);
    }

    return m_typelist;
}

DocumentsPtr Engine::GetDocuments(const Date& from,
                                  const Date& to,
                                  const AccountId accountId,
                                  const CurrencyId currencyId,
                                  const hb::DocTypeIdList& doc_types)
{
    const auto filter = m_storage->GetFilterFactory().CreateDocByDateFilter(
        from, to, accountId, currencyId, doc_types);
    return m_storage->GetDocuments(*filter);
}

AccountMapPtr Engine::GetAccounts(bool reload)
{
    if (reload || !m_accounts)
    {
        static const auto filter = m_storage->GetFilterFactory().CreateAllAccountsFilter();
        m_accounts = m_storage->GetAccounts(*filter);
    }

    return m_accounts;
}

namespace
{
class AccountOrderComporator
{
public:
    bool operator()(const hb::AccountPtr& first, const hb::AccountPtr& second)
    {
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
        hb::AccountMapPtr accounts = Engine::GetInstance().GetAccounts(reload);

        m_accountList->reserve(accounts->size());

        for (hb::AccountMap::const_iterator it = accounts->begin(); it != accounts->end();
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
        static const auto filter = m_storage->GetFilterFactory().CreateAllCurrencyFilter();
        m_currencies = m_storage->GetCurrencies(*filter);
    }

    return m_currencies;
}

BalancePtr Engine::GetBalance(const Date& date)
{
    const auto filter = m_storage->GetFilterFactory().CreateBalanceFilter(date);
    return m_storage->GetBalance(*filter);
}

PaymentsBalancePtr Engine::GetPaymentsBalance(const Date& date)
{
    const auto filter = m_storage->GetFilterFactory().CreatePaymentsBalanceFilter(date);
    return m_storage->GetPaymentsBalance(*filter);
}

PaymentTypesMapPtr Engine::GetPaymentTypes(bool reload)
{
    if (reload || !m_paymentTypes)
    {
        static const auto filter = m_storage->GetFilterFactory().CreatePaymentsBalanceFilter();
        m_paymentTypes = m_storage->GetPaymentTypes(*filter);
    }

    return m_paymentTypes;
}

PaymentsPtr Engine::GetPayments(const Date& from, const Date& to)
{
    const auto filter = m_storage->GetFilterFactory().CreatePaymentsByDateFilter(from, to);
    return m_storage->GetPayments(*filter);
}

ShopListPtr Engine::GetShops(bool reload)
{
    if (reload || !m_shops)
    {
        static const auto filter = m_storage->GetFilterFactory().CreateShopListFilter();
        m_shops = m_storage->GetShopList(*filter);
    }

    return m_shops;
}

ReportPtr Engine::GetReport(const Date& from, const Date& to) const
{
    static const auto filter = m_storage->GetFilterFactory().CreateReportFilter(from, to);
    return m_storage->GetReport(*filter);
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

}  // namespace hb
