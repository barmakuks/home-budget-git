#ifndef SETVALUEFUNCTIONS_H
#define SETVALUEFUNCTIONS_H

#include <string>

namespace hb
{

namespace core
{
class Account;
class BalanceRow;
class Currency;
class Document;
class DocumentType;
class PaymentDocument;
class PaymentsBalanceRow;
class PaymentType;
}

namespace storage
{

void SetAccountValue(hb::core::Account& account, const std::string& field, const std::string& value);
void SetBalanceValue(hb::core::BalanceRow& balance, const std::string& field, const std::string& value);
void SetCurrencyValue(hb::core::Currency& currency, const std::string& field, const std::string& value);
void SetDocumentValue(hb::core::Document& doc, const std::string& field, const std::string& value);
void SetDocTypeValue(hb::core::DocumentType& docType, const std::string& field, const std::string& value);
void SetShopValue(std::string& shop, const std::string& field, const std::string& value);
void SetPaymentValue(hb::core::PaymentDocument& doc, const std::string& field, const std::string& value);
void SetPaymentsBalanceValue(hb::core::PaymentsBalanceRow& balance, const std::string& field, const std::string& value);
void SetPaymentTypeValue(hb::core::PaymentType& paymentType, const std::string& field, const std::string& value);

} // namespace storage
} // namespace hb


#endif // SETVALUEFUNCTIONS_H
