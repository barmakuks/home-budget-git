#ifndef ISTORAGE_H
#define ISTORAGE_H

#include "documenttype.h"
#include "document.h"
#include "account.h"
#include "currency.h"
#include "balance.h"

namespace hb
{
namespace core
{

class IFilter
{
public:
    virtual std::string Fields() const = 0;
    virtual std::string From() const = 0;
    virtual std::string WhereCondition() const = 0;
    virtual std::string GroupByCondition() const = 0;
    virtual std::string HavingCondition() const = 0;
    virtual std::string OrderBy() const = 0;
};

class IStorage
{
public:
    virtual ~IStorage(){}

    /** Returns map of pairs [DocumentId, DocumentType] from storage
      @param sign
    */
    virtual DocumentTypeListPtr GetTypeList(const IFilter& filter) const = 0;

    virtual DocumentsPtr GetDocuments(const IFilter& filter) const = 0;

    virtual AccountMapPtr GetAccounts(const IFilter& filter) const = 0;

    virtual CurrencyMapPtr GetCurrencies(const IFilter& filter) const = 0;

    virtual BalancePtr GetBalance(const IFilter& filter) const  = 0;

    virtual bool Write(Document& doc) const = 0;
    virtual bool Write(DocumentType& docType) const = 0;
    virtual bool Write(Account& account) const = 0;
    virtual bool Write(Currency& currency) const = 0;

};

} // namespace core
} // namespace hb

#endif // ISTORAGE_H
