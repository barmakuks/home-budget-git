#include "gtest/gtest.h"

#include "documenttype.h"
#include "currency.h"
#include "account.h"

#include "sql-builder.h"

using namespace hb::core;
using namespace hb::storage;

TEST(SqlBuilderSuite, testInsertCurrencyStatement)
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

TEST(SqlBuilderSuite, testInsertDocumentTypeStatement)
{
    using namespace hb::core;
    DocumentType docType;
    docType.SetParentId(1);
    docType.SetSign(DocumentType::Debit);
    docType.SetName("Test name");

    const std::string expectedStatement = "INSERT INTO doc_types (parent_id, operation_sign, name) VALUES (1, -1, 'Test name')";

    ASSERT_EQ(expectedStatement, BuildSql(docType));
}

TEST(SqlBuilderSuite, testUpdateDocumentTypeStatement)
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

TEST(SqlBuilderSuite, testInsertAccountStatement)
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

TEST(SqlBuilderSuite, testUpdateAccountStatement)
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

