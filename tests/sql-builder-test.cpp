#include "gtest/gtest.h"

#include "documenttype.h"
#include "currency.h"
#include "account.h"
#include "document.h"

#include "sql-builder.h"

using namespace hb::core;
using namespace hb::storage;

TEST(SqlBuilderSuite, testInsertCurrencySql)
{
    Currency cur;
    cur.SetCode(980);
    cur.SetIsoCode("UAH");
    cur.SetShortName("uah");
    cur.SetSymbol("$");
    cur.SetSymbolPlace(false);

    const std::string expectedStatement = "INSERT INTO currency_list (code, iso_code, short_name, symbol, symbol_place) VALUES (980, 'UAH', 'uah', '$', 0)";

    // TODO : implement update statement

    ASSERT_EQ(expectedStatement, BuildSql(cur));
}

TEST(SqlBuilderSuite, testInsertDocumentTypeSql)
{
    using namespace hb::core;
    DocumentType docType;
    docType.SetParentId(1);
    docType.SetSign(DocumentType::Debit);
    docType.SetName("Test name");

    const std::string expectedStatement = "INSERT INTO doc_types (parent_id, operation_sign, name) VALUES (1, -1, 'Test name')";

    ASSERT_EQ(expectedStatement, BuildSql(docType));
}

TEST(SqlBuilderSuite, testUpdateDocumentTypeSql)
{
    using namespace hb::core;
    DocumentType docType;
    docType.SetId(2);
    docType.SetParentId(1);
    docType.SetSign(DocumentType::Debit);
    docType.SetName("Test name");

    const std::string expectedStatement = "UPDATE doc_types SET parent_id=1, operation_sign=-1, name='Test name' WHERE id=2";

    ASSERT_EQ(expectedStatement, BuildSql(docType));
}

TEST(SqlBuilderSuite, testInsertAccountSql)
{
    using namespace hb::core;
    Account account;
    account.SetName("Test name");
    account.SetDescription("Test description");
    account.SetDefaultCurrency(980);
    account.SetSortOrder(2);
    account.SetBank("Test bank");
    account.SetActive(true);
    account.SetForegroundColor(12345678);

    const std::string expectedStatement = "INSERT INTO accounts (name, description, default_currency_code, sort_order, bank, is_active, foreground_color) "
            "VALUES ('Test name', 'Test description', 980, 2, 'Test bank', 1, 12345678)";

    ASSERT_EQ(expectedStatement, BuildSql(account));
}

TEST(SqlBuilderSuite, testUpdateAccountSql)
{
    using namespace hb::core;
    Account account;
    account.SetId(1);
    account.SetName("Test name");
    account.SetDescription("Test description");
    account.SetDefaultCurrency(980);
    account.SetSortOrder(2);
    account.SetBank("Test bank");
    account.SetActive(true);
    account.SetForegroundColor(12345678);

    const std::string expectedStatement = "UPDATE accounts SET name='Test name', description='Test description', default_currency_code=980, "
            "sort_order=2, bank='Test bank', is_active=1, foreground_color=12345678 "
            "WHERE id=1";

    ASSERT_EQ(expectedStatement, BuildSql(account));
}

TEST(SqlBuilderSuite, testInsertDocumentFrom)
{
    using namespace hb::core;
    Document doc;
    doc.SetDocDate("20140813");
    doc.SetDocType(1);
    doc.SetNote("Test note");
    doc.SetShop("Test shop");

    Amount amount;
    amount.SetAccount(1);
    amount.SetCurrency(980);
    amount.SetValue(100000);
    doc.SetAmountFrom(amount);

    const std::string expectedStatement = "INSERT INTO documents (doc_date, doc_type_id, account_from_id, account_from_cur, amount_from, account_to_id, account_to_cur, amount_to, note, shop) "
            "VALUES ('20140813', 1, 1, 980, 100000, NULL, NULL, NULL, 'Test note', 'Test shop')";

    ASSERT_EQ(expectedStatement, BuildSql(doc));
}

TEST(SqlBuilderSuite, testInsertDocumentTo)
{
    using namespace hb::core;
    Document doc;
    doc.SetDocDate("20140813");
    doc.SetDocType(1);
    doc.SetNote("Test note");
    doc.SetShop("Test shop");

    Amount amount;
    amount.SetAccount(1);
    amount.SetCurrency(980);
    amount.SetValue(100000);
    doc.SetAmountTo(amount);

    const std::string expectedStatement = "INSERT INTO documents (doc_date, doc_type_id, account_from_id, account_from_cur, amount_from, account_to_id, account_to_cur, amount_to, note, shop) "
            "VALUES ('20140813', 1, NULL, NULL, NULL, 1, 980, 100000, 'Test note', 'Test shop')";

    ASSERT_EQ(expectedStatement, BuildSql(doc));
}

TEST(SqlBuilderSuite, testInsertDocumentFromTo)
{
    using namespace hb::core;
    Document doc;
    doc.SetDocDate("20140813");
    doc.SetDocType(1);
    doc.SetNote("Test note");
    doc.SetShop("Test shop");

    Amount amount_from;
    amount_from.SetAccount(1);
    amount_from.SetCurrency(980);
    amount_from.SetValue(100000);
    doc.SetAmountFrom(amount_from);

    Amount amount_to;
    amount_to.SetAccount(2);
    amount_to.SetCurrency(980);
    amount_to.SetValue(100000);
    doc.SetAmountTo(amount_to);

    const std::string expectedStatement = "INSERT INTO documents "
            "(doc_date, doc_type_id, account_from_id, account_from_cur, amount_from, account_to_id, account_to_cur, amount_to, note, shop) "
            "VALUES ('20140813', 1, 1, 980, 100000, 2, 980, 100000, 'Test note', 'Test shop')";

    ASSERT_EQ(expectedStatement, BuildSql(doc));
}

TEST(SqlBuilderSuite, testUpdateDocumentFrom)
{
    using namespace hb::core;
    Document doc;
    doc.SetId(18553);
    doc.SetDocDate("20140814");
    doc.SetDocType(2);
    doc.SetNote("Test note 1");
    doc.SetShop("Test shop 2");

    Amount amount_from;
    amount_from.SetAccount(1);
    amount_from.SetCurrency(980);
    amount_from.SetValue(150000);
    doc.SetAmountFrom(amount_from);

    doc.ResetAmountTo();

    const std::string expectedStatement = "UPDATE documents SET doc_date='20140814', doc_type_id=2, "
            "account_from_id=1, account_from_cur=980, amount_from=150000, "
            "account_to_id=NULL, account_to_cur=NULL, amount_to=NULL, "
            "note='Test note 1', shop='Test shop 2' "
            "WHERE id=18553";

    ASSERT_EQ(expectedStatement, BuildSql(doc));
}

TEST(SqlBuilderSuite, testUpdateDocumentTo)
{
    using namespace hb::core;
    Document doc;
    doc.SetId(18553);
    doc.SetDocDate("20140814");
    doc.SetDocType(2);
    doc.SetNote("Test note 1");
    doc.SetShop("Test shop 2");

    Amount amount;
    amount.SetAccount(1);
    amount.SetCurrency(980);
    amount.SetValue(150000);
    doc.SetAmountTo(amount);

    doc.ResetAmountFrom();

    const std::string expectedStatement = "UPDATE documents SET doc_date='20140814', doc_type_id=2, "
            "account_from_id=NULL, account_from_cur=NULL, amount_from=NULL, "
            "account_to_id=1, account_to_cur=980, amount_to=150000, "
            "note='Test note 1', shop='Test shop 2' "
            "WHERE id=18553";

    ASSERT_EQ(expectedStatement, BuildSql(doc));
}
TEST(SqlBuilderSuite, testUpdateDocumentFromTo)
{
    using namespace hb::core;
    Document doc;
    doc.SetId(18553);
    doc.SetDocDate("20140814");
    doc.SetDocType(2);
    doc.SetNote("Test note 1");
    doc.SetShop("Test shop 2");

    Amount amount_from;
    amount_from.SetAccount(1);
    amount_from.SetCurrency(980);
    amount_from.SetValue(150000);
    doc.SetAmountFrom(amount_from);

    Amount amount_to;
    amount_to.SetAccount(1);
    amount_to.SetCurrency(980);
    amount_to.SetValue(150000);
    doc.SetAmountTo(amount_to);

    const std::string expectedStatement = "UPDATE documents SET doc_date='20140814', doc_type_id=2, "
            "account_from_id=1, account_from_cur=980, amount_from=150000, "
            "account_to_id=1, account_to_cur=980, amount_to=150000, "
            "note='Test note 1', shop='Test shop 2' "
            "WHERE id=18553";

    ASSERT_EQ(expectedStatement, BuildSql(doc));
}
