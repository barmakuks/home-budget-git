#ifndef MODEL_H
#define MODEL_H

#include <memory>

#include "istorage.h"

namespace hb
{
namespace core
{

class Model
{
public:
    static Model& CreateInstance(const IStoragePtr& storage);
    static Model& GetInstance();

public:

    DocumentTypeListPtr GetTypeList(DocumentType::TypeSign documentType);
    DocumentTypeListPtr GetTypeList();

    DocumentsPtr GetDocuments(const Date& from, const Date& to);

    AccountMapPtr GetAccounts();

    CurrencyMapPtr GetCurrencies();

    BalancePtr GetBalance(const Date& date);

protected:
    Model(const IStoragePtr& storage);

private:
    static std::unique_ptr<Model>  m_model;

private:
    IStoragePtr m_storage;
};

} // namespace core
} // namespace hb
#endif // MODEL_H
