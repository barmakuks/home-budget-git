#include "set-value-functions.h"

#include "setters.hpp"
#include "account.h"
#include "balance.h"
#include "currency.h"
#include "document.h"
#include "documenttype.h"
#include "payments-balance.h"
#include "payment-document.h"
#include "payment-type.h"

namespace hb
{
namespace storage
{

void SetAccountValue(core::Account& account, const std::string& field, const std::string& value)
{
    using namespace hb::core;
    std::string name = field;
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    SetFieldValue(account, &Account::SetId, "id", name, value);
    SetFieldValue(account, &Account::SetName, "name", name, value);
    SetFieldValue(account, &Account::SetDescription, "description", name, value);
    SetFieldValue(account, &Account::SetDefaultCurrency, "default_currency_code", name, value);
    SetFieldValue(account, &Account::SetSortOrder, "sort_order", name, value);
    SetFieldValue(account, &Account::SetBank, "bank", name, value);
    SetFieldValue(account, &Account::SetActive, "is_active", name, value);
    SetFieldValue(account, &Account::SetForegroundColor, "foreground_color", name, value);
}

void SetBalanceValue(core::BalanceRow& balance, const std::string& field, const std::string& value)
{
    using namespace hb::core;

    std::string name = field;
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    SetFieldValue(balance, &BalanceRow::SetAccount, "account_id", name, value);
    SetFieldValue(balance, &BalanceRow::SetCurrency, "account_cur", name, value);
    SetFieldValue(balance, &BalanceRow::SetAmount, "amount", name, value);
}

void SetCurrencyValue(core::Currency& currency, const std::string& field, const std::string& value)
{
    using namespace hb::core;
    std::string name = field;
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    SetFieldValue(currency, &Currency::SetId, "code", name, value);
    SetFieldValue(currency, &Currency::SetCode, "code", name, value);
    SetFieldValue(currency, &Currency::SetIsoCode, "iso_code", name, value);
    SetFieldValue(currency, &Currency::SetShortName, "short_name", name, value);
    SetFieldValue(currency, &Currency::SetSymbol, "symbol", name, value);
    SetFieldValue(currency, &Currency::SetSymbolPlace, "symbol_place", name, value);
    SetFieldValue(currency, &Currency::SetForegroundColor, "fg_color", name, value);
}

void SetDocumentValue(hb::core::Document& doc, const std::string& field, const std::string& value)
{
    using namespace hb::core;
    std::string name = field;
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    SetFieldValue(doc, &Document::SetId, "id", name, value);
    SetFieldValue(doc, &Document::SetDocDate, "doc_date", name, value);
    SetFieldValue(doc, &Document::SetDocType, "doc_type_id", name, value);
    SetFieldValue(doc, &Document::SetShop, "shop", name, value);
    SetFieldValue(doc, &Document::SetNote, "note", name, value);

    if (!value.empty() && (name == "account_from_id" || name == "account_from_cur" || name == "amount_from"))
    {
        if (!doc.AmountFrom().is_initialized())
        {
            doc.SetAmountFrom(Amount());
        }

        Amount amount = doc.AmountFrom().get();
        SetFieldValue(amount, &Amount::SetAccount, "account_from_id", name, value);
        SetFieldValue(amount, &Amount::SetCurrency, "account_from_cur", name, value);
        SetFieldValue(amount, &Amount::SetValue, "amount_from", name, value);
        doc.SetAmountFrom(amount);
    }


    if (!value.empty() && (name == "account_to_id" || name == "account_to_cur" || name == "amount_to"))
    {
        if (!doc.AmountTo().is_initialized())
        {
            doc.SetAmountTo(Amount());
        }
        Amount amount = doc.AmountTo().get();
        SetFieldValue(amount, &Amount::SetAccount, "account_to_id", name, value);
        SetFieldValue(amount, &Amount::SetCurrency, "account_to_cur", name, value);
        SetFieldValue(amount, &Amount::SetValue, "amount_to", name, value);
        doc.SetAmountTo(amount);
    }

}

void SetDocTypeValue(core::DocumentType& docType, const std::string& field, const std::string& value)
{
    using namespace hb::core;
    std::string name = field;
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    SetFieldValue(docType, &DocumentType::SetId, "id", name, value);
    SetFieldValue(docType, &DocumentType::SetName, "name", name, value);
    SetFieldValue(docType, &DocumentType::SetParentId, "parent_id", name, value);
    SetFieldValue(docType, &DocumentType::SetSign, "operation_sign", name, value);
}

void SetShopValue(std::string& shop, const std::string& field, const std::string& value)
{
    std::string name = field;
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    if (name == "shop")
    {
        shop = value;
    }
}

void SetPaymentValue(hb::core::PaymentDocument& doc, const std::string& field, const std::string& value)
{
    using namespace hb::core;
    std::string name = field;
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    SetFieldValue(doc, &PaymentDocument::SetId, "id", name, value);
    SetFieldValue(doc, &PaymentDocument::SetPaymentType, "payment_id", name, value);
    SetFieldValue(doc, &PaymentDocument::SetAmount, "amount", name, value);

    SetFieldValue(doc, &PaymentDocument::SetDocDate, "date", name, value);
    SetFieldValue(doc, &PaymentDocument::SetPeriod, "period", name, value);
    SetFieldValue(doc, &PaymentDocument::SetCounter1, "counter_1", name, value);
    SetFieldValue(doc, &PaymentDocument::SetCounter2, "counter_2", name, value);
    SetFieldValue(doc, &PaymentDocument::SetNote, "note", name, value);
}

void SetPaymentsBalanceValue(hb::core::PaymentsBalanceRow& balance, const std::string& field, const std::string& value)
{
    using namespace hb::core;
    std::string name = field;
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    SetFieldValue(balance, &PaymentsBalanceRow::SetPaymentType, "id", name, value);
    SetFieldValue(balance, &PaymentsBalanceRow::SetPaymentName, "name", name, value);
    SetFieldValue(balance, &PaymentsBalanceRow::SetAmount, "amount", name, value);
}

void SetPaymentTypeValue(hb::core::PaymentType& paymentType, const std::string& field, const std::string& value)
{
    using namespace hb::core;
    std::string name = field;
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    SetFieldValue(paymentType, &PaymentType::SetId, "id", name, value);
    SetFieldValue(paymentType, &PaymentType::SetName, "name", name, value);
}

} // namespace storage
} // namespace hb
