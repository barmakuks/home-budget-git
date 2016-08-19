#pragma once
#include <memory>

#include "storage/storage.h"
#include <documenttype.h>
#include "payment-type.h"

namespace hb
{
class Document;
class Account;
class Currency;

class DocEngine
{
    DocEngine(const DocEngine& storage) = delete;

public:
    DocEngine(const storage::StoragePtr& storage);

    DocumentPtr CreateDocument(DocumentType::Direction docType);

    DocumentTypeListPtr GetTypeList(DocumentType::Direction documentType);

    DocumentTypeListPtr GetTypeList(bool reload = false);

    DocumentsPtr GetDocuments(const Date& from,
                              const Date& to,
                              const AccountId accountId,
                              const CurrencyId currencyId,
                              const DocTypeIdList& doc_types);

    AccountMapPtr GetAccounts(bool reload = false);
    AccountListPtr GetAccountsList(bool reload = false);

    CurrencyMapPtr GetCurrencies(bool reload = false);
    BalancePtr GetBalance(const Date& date);

    PaymentsBalancePtr GetPaymentsBalance(const Date& date);

    PaymentTypesMapPtr GetPaymentTypes(bool reload = false);

    PaymentsPtr GetPayments(const Date& from, const Date& to);

    ShopListPtr GetShops(bool reload = false);

    ReportPtr GetReport(const Date& from, const Date& to) const;

    bool Write(Document& doc);
    bool Write(DocumentType& docType);
    bool Write(Account& account);
    bool Write(Currency& currency);

    bool DeleteDocument(DocId id);
    bool DeleteDocumentType(DocTypeId id);
    bool DeleteAccount(AccountId id);
    bool DeleteCurrency(CurrencyId code);

protected:
    DocTypeId GetRootDocTypeId(DocumentType::Direction documentType);

private:
    storage::StoragePtr m_storage;

    AccountMapPtr m_accounts;
    AccountListPtr m_accountList;
    CurrencyMapPtr m_currencies;
    DocumentTypeListPtr m_typelist;
    ShopListPtr m_shops;
    PaymentTypesMapPtr m_paymentTypes;

    typedef std::map<DocumentType::Direction, DocTypeId> DocTypeRootsMap;
    DocTypeRootsMap m_docTypeRoots;
};

}  // namespace hb
