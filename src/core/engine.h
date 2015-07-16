#ifndef ENGINE_H
#define ENGINE_H

#include <memory>

#include "istorage.h"

#include "payment-type.h"

namespace hb
{
namespace core
{

class Document;
class DocumentType;
class Account;
class Currency;

class Engine
{
public:
    static Engine& CreateInstance(const IStoragePtr& storage);
    static Engine& GetInstance();

public:

    DocumentPtr CreateDocument(DocumentType::TypeSign docType);

    DocumentTypeListPtr GetTypeList(DocumentType::TypeSign documentType);

    DocumentTypeListPtr GetTypeList(bool reload = false);

    DocumentsPtr GetDocuments(const Date& from,
                              const Date& to,
                              const hb::AccountId accountId,
                              const hb::CurrencyId currencyId);

    AccountMapPtr GetAccounts(bool reload = false);
    AccountListPtr GetAccountsList(bool reload = false);

    CurrencyMapPtr GetCurrencies(bool reload = false);
    BalancePtr GetBalance(const Date& date);

    PaymentsBalancePtr GetPaymentsBalance(const Date& date);

    PaymentTypesMapPtr GetPaymentTypes(bool reload = false);

    PaymentsPtr GetPayments(const Date& from,
                            const Date& to);

    ShopListPtr GetShops(bool reload = false);

    ReportPtr GetReport(const Date& from,
                             const Date& to) const;

    bool Write(Document& doc);
    bool Write(DocumentType& docType);
    bool Write(Account& account);
    bool Write(Currency& currency);

    bool DeleteDocument(DocId id);
    bool DeleteDocumentType(DocTypeId id);
    bool DeleteAccount(AccountId id);
    bool DeleteCurrency(CurrencyId code);

protected:
    Engine(const IStoragePtr& storage);

    DocTypeId GetRootDocTypeId(DocumentType::TypeSign documentType);
private:
    static std::unique_ptr<Engine>  m_engine;

private:
    IStoragePtr m_storage;

    AccountMapPtr       m_accounts;
    AccountListPtr      m_accountList;
    CurrencyMapPtr      m_currencies;
    DocumentTypeListPtr m_typelist;
    ShopListPtr         m_shops;
    PaymentTypesMapPtr  m_paymentTypes;

    typedef std::map<DocumentType::TypeSign, DocTypeId> DocTypeRootsMap;
    DocTypeRootsMap     m_docTypeRoots;
};

} // namespace core
} // namespace hb
#endif // ENGINE_H
