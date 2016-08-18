#pragma once

#include <string>

namespace hb
{

class Account;
class BalanceRow;
class Currency;
class Document;
class DocumentType;
class PaymentDocument;
class PaymentsBalanceRow;
class PaymentType;

namespace sql_storage
{

void SetAccountValue(hb::Account& account, const std::string& field, const std::string& value);
void SetBalanceValue(hb::BalanceRow& balance, const std::string& field, const std::string& value);
void SetCurrencyValue(hb::Currency& currency, const std::string& field, const std::string& value);
void SetDocumentValue(hb::Document& doc, const std::string& field, const std::string& value);
void SetDocTypeValue(hb::DocumentType& docType, const std::string& field, const std::string& value);
void SetShopValue(std::string& shop, const std::string& field, const std::string& value);
void SetPaymentValue(hb::PaymentDocument& doc, const std::string& field, const std::string& value);
void SetPaymentsBalanceValue(hb::PaymentsBalanceRow& balance, const std::string& field, const std::string& value);
void SetPaymentTypeValue(hb::PaymentType& paymentType, const std::string& field, const std::string& value);

} // namespace sql_storage
} // namespace hb
