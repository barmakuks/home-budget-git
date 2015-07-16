#include "istorage.h"
#include "sqlite-engine.h"
#include "database-storage.h"
#include "doc-type-sign-filter.h"
#include "document-by-date-filter.h"
#include "all-accounts-filter.h"
#include "all-currency-filter.h"
#include "account.h"
#include "currency.h"
#include "balance.h"
#include "balance-filter.h"
#include <iostream>

using namespace std;

void Print(hb::core::DocumentTypeList& list, hb::core::DocTypeIdList& head, int level = 0)
{
    using namespace hb::core;

    hb::core::DocTypeIdList doc_types = head;

    SortByName(doc_types, list);

    for (DocTypeIdList::const_iterator it = doc_types.begin();
         it != doc_types.end();
         ++it)
    {
        for (int i = 0; i < level; ++i)
        {
            cout << "    ";
        }

        DocumentTypeList::const_iterator type = list.find(*it);

        if (type != list.end())
        {
            cout << type->second->Name() << endl;
            Print(list, type->second->Subtypes(), level + 1);
        }
    }
}

void Print(const hb::core::Documents& documents, const hb::core::DocumentTypeList& docTypes)
{
    using namespace hb::core;

    for (Documents::const_iterator it = documents.begin();
         it != documents.end();
         ++it)
    {
//        DocumentPtr doc = it->second;
        DocumentPtr doc = *it;

        std::string doc_type_name = "\t";
        DocumentTypeList::const_iterator dtit = docTypes.find(doc->DocType());
        if (dtit != docTypes.end())
        {
            doc_type_name = dtit->second->Name();
        }

        cout << "ID: " << doc->Id() << " DATE: " << doc->DocDate() << " " << doc->DocType() << " " << doc_type_name << " FROM ";
        const bool amount_from_exists = doc->AmountFrom().is_initialized();

        if (amount_from_exists)
        {
            std::cout << doc->AmountFrom()->Value() << " " << doc->AmountFrom()->Currency();
        }
        else
        {
            std::cout << "\t";
        }
        std::cout << std::endl;
    }
}

void Print(const hb::core::AccountMap& accounts)
{
    using namespace hb::core;

    for (AccountMap::const_iterator it = accounts.begin();
         it != accounts.end();
         ++it)
    {
        AccountPtr account = it->second;

        cout << "ID: " << account->Id() << ": " << account->Name() << ". " << account->Description();
        std::cout << std::endl;
    }
}

void Print(const hb::core::CurrencyMap& currency_list)
{
    using namespace hb::core;

    for (CurrencyMap::const_iterator it = currency_list.begin();
         it != currency_list.end();
         ++it)
    {
        CurrencyPtr currency = it->second;

        cout << currency->Code() << ": " << currency->IsoCode() << ". " << currency->ShortName();
        std::cout << std::endl;
    }
}

void Print(const hb::core::Balance& balance,
           const hb::core::AccountMap& accounts,
           const hb::core::CurrencyMap& currency_list)
{
    using namespace hb::core;

    for (Balance::const_iterator it = balance.begin();
         it != balance.end();
         ++it)
    {
        BalanceRowPtr item = *it;

        cout << accounts.at(item->Account())->Name() << " [" << currency_list.at(item->Currency())->ShortName() << "]:\t" << item->Amount();
        std::cout << std::endl;
    }
}

void RunDataBaseTest()
{
    using namespace hb::core;
    using namespace hb::sqlite;
    using namespace hb::storage;


    SqliteEngine engine("/home/vitalii/development/barma-home-budget/data/budget.sqlite");

    DatabaseStorage storage(engine);


    cout << "Hello World!" << endl;

    DocumentTypeListPtr docTypes = storage.GetTypeList(DocTypeSignFilter(DocumentType::TypeSign::Outcome));
//    const DocTypeIdList& head = docTypes->Head();
//    Print(*docTypes, head);

    DocumentsPtr documents = storage.GetDocuments(DocByDateFilter("20140101"));

    Print(*documents, *docTypes);

//    AccountMapPtr accounts = storage.GetAccounts(AllAccountsFilter());
//    Print(*accounts);

//    CurrencyMapPtr currencies = storage.GetCurrencies(AllCurrencyFilter());
//    Print(*currencies);

//    BalanceSetPtr balance = storage.GetBalance(BalanceFilter("20130101"));
//    Print(*balance, *accounts, *currencies);
}
