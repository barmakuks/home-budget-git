#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <memory>
#include <boost/optional.hpp>

#include "documenttype.h"
#include "raw-types.h"

namespace hb
{
namespace core
{

class Amount
{
public:
    Amount():
        m_account_id(0),
        m_currency_id(0),
        m_value(0)
    {
    }

    AccountId Account() const {return m_account_id; }
    void SetAccount(AccountId account) { m_account_id = account; }

    CurrencyId Currency() const {return m_currency_id; }
    void SetCurrency(CurrencyId currency) { m_currency_id = currency; }

    Money Value() const {return m_value; }
    void SetValue(Money value) {m_value = value;}

private:
    AccountId   m_account_id;
    CurrencyId  m_currency_id;
    Money       m_value;
};

class Document
{
public:

    Document();

    DocId Id() const { return m_id; }
    void SetId(DocId documentId) { m_id = documentId; }

    Date DocDate() const { return m_doc_date; }
    void SetDocDate(const Date& date) { m_doc_date = date; }

    DocTypeId DocType() const { return m_doc_type_id; }
    void SetDocType(const DocTypeId doc_type_id) { m_doc_type_id = doc_type_id; }

    boost::optional<Amount> AmountFrom() const { return m_amount_from; }
    void SetAmountFrom(const Amount& amount) { m_amount_from = amount; }
    void ResetAmountFrom(){m_amount_from.reset();}

    boost::optional<Amount> AmountTo() const { return m_amount_to; }
    void SetAmountTo(const Amount& amount) { m_amount_to = amount; }
    void ResetAmountTo(){m_amount_to.reset();}

    std::string Note() const { return m_note; }
    void SetNote(const std::string& note) { m_note = note; }

    std::string Shop() const { return m_shop; }
    void SetShop(const std::string& shop) { m_shop = shop; }

private:
    DocId   m_id;
    Date m_doc_date;
    DocTypeId m_doc_type_id;

    boost::optional<Amount> m_amount_from;
    boost::optional<Amount> m_amount_to;

    std::string m_note;
    std::string m_shop;
};

typedef std::shared_ptr<Document> DocumentPtr;

typedef std::map<DocId, DocumentPtr> DocumentsMap;
typedef std::shared_ptr<DocumentsMap> DocumentsMapPtr;

} // namespace core
} // namespace hb

#endif // DOCUMENT_H
