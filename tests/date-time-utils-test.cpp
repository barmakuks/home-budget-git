#include "gtest/gtest.h"

#include "date-time-utils.h"

using namespace hb::utils;

TEST(DateTimeUtilsSuite, testToday)
{
    std::string expectedValue = "20140904";

    DateInterval interval = GetDateInterval(DatePeriod::Today, "20140904");
    ASSERT_EQ(expectedValue, interval.from);
    ASSERT_EQ(expectedValue, interval.to);
}

TEST(DateTimeUtilsSuite, testYesterday)
{
    std::string expectedValue = "20140903";

    DateInterval interval = GetDateInterval(DatePeriod::Yesterday, "20140904");
    ASSERT_EQ(expectedValue, interval.from);
    ASSERT_EQ(expectedValue, interval.to);
}

TEST(DateTimeUtilsSuite, testThisWeek)
{
    std::string expectedFromValue = "20140901";
    std::string expectedToValue = "20140907";

    DateInterval interval = GetDateInterval(DatePeriod::ThisWeek, "20140904");
    ASSERT_EQ(expectedFromValue, interval.from);
    ASSERT_EQ(expectedToValue, interval.to);
}

TEST(DateTimeUtilsSuite, testLastWeek)
{
    std::string expectedFromValue = "20140825";
    std::string expectedToValue = "20140831";

    DateInterval interval = GetDateInterval(DatePeriod::LastWeek, "20140904");
    ASSERT_EQ(expectedFromValue, interval.from);
    ASSERT_EQ(expectedToValue, interval.to);
}

TEST(DateTimeUtilsSuite, testThisMonth)
{
    std::string expectedFromValue = "20140901";
    std::string expectedToValue = "20140930";

    DateInterval interval = GetDateInterval(DatePeriod::ThisMonth, "20140904");
    ASSERT_EQ(expectedFromValue, interval.from);
    ASSERT_EQ(expectedToValue, interval.to);
}

TEST(DateTimeUtilsSuite, testThisMonthFebLeapYear)
{
    std::string expectedFromValue = "20040201";
    std::string expectedToValue = "20040229";

    DateInterval interval = GetDateInterval(DatePeriod::ThisMonth, "20040215");
    ASSERT_EQ(expectedFromValue, interval.from);
    ASSERT_EQ(expectedToValue, interval.to);
}

TEST(DateTimeUtilsSuite, testThisMonthFebCommonYear)
{
    std::string expectedFromValue = "20140201";
    std::string expectedToValue = "20140228";

    DateInterval interval = GetDateInterval(DatePeriod::ThisMonth, "20140215");
    ASSERT_EQ(expectedFromValue, interval.from);
    ASSERT_EQ(expectedToValue, interval.to);
}

TEST(DateTimeUtilsSuite, testLastMonth)
{
    std::string expectedFromValue = "20140801";
    std::string expectedToValue = "20140831";

    DateInterval interval = GetDateInterval(DatePeriod::LastMonth, "20140904");
    ASSERT_EQ(expectedFromValue, interval.from);
    ASSERT_EQ(expectedToValue, interval.to);
}

TEST(DateTimeUtilsSuite, testLastMonthLastYear)
{
    std::string expectedFromValue = "20131201";
    std::string expectedToValue =   "20131231";

    DateInterval interval = GetDateInterval(DatePeriod::LastMonth, "20140115");
    ASSERT_EQ(expectedFromValue, interval.from);
    ASSERT_EQ(expectedToValue, interval.to);
}

TEST(DateTimeUtilsSuite, testThisQuarter)
{
    std::string expectedFromValue = "20140701";
    std::string expectedToValue = "20140930";

    DateInterval interval = GetDateInterval(DatePeriod::ThisQuarter, "20140904");
    ASSERT_EQ(expectedFromValue, interval.from);
    ASSERT_EQ(expectedToValue, interval.to);
}

TEST(DateTimeUtilsSuite, testLastQuarter)
{
    std::string expectedFromValue = "20140401";
    std::string expectedToValue = "20140630";

    DateInterval interval = GetDateInterval(DatePeriod::LastQuarter, "20140904");
    ASSERT_EQ(expectedFromValue, interval.from);
    ASSERT_EQ(expectedToValue, interval.to);
}

TEST(DateTimeUtilsSuite, testLastQuarterLastYear)
{
    std::string expectedFromValue = "20131001";
    std::string expectedToValue = "20131231";

    DateInterval interval = GetDateInterval(DatePeriod::LastQuarter, "20140115");
    ASSERT_EQ(expectedFromValue, interval.from);
    ASSERT_EQ(expectedToValue, interval.to);
}

TEST(DateTimeUtilsSuite, testThisYear)
{
    std::string expectedFromValue = "20140101";
    std::string expectedToValue = "20141231";

    DateInterval interval = GetDateInterval(DatePeriod::ThisYear, "20140904");
    ASSERT_EQ(expectedFromValue, interval.from);
    ASSERT_EQ(expectedToValue, interval.to);
}

TEST(DateTimeUtilsSuite, testLastYear)
{
    std::string expectedFromValue = "20130101";
    std::string expectedToValue = "20131231";

    DateInterval interval = GetDateInterval(DatePeriod::LastYear, "20140904");
    ASSERT_EQ(expectedFromValue, interval.from);
    ASSERT_EQ(expectedToValue, interval.to);
}

/***********************************************************************************************************************/

TEST(DateTimeUtilsSuite, testGetPeriodToday)
{
    const DateInterval interval("20140904", "20140904");

    DatePeriod::Period period = GetDatePeriod(interval, "20140904");
    ASSERT_EQ(DatePeriod::Today, period);
}

TEST(DateTimeUtilsSuite, testGetPeriodYesterday)
{
    const DateInterval interval("20140903", "20140903");

    DatePeriod::Period period = GetDatePeriod(interval, "20140904");
    ASSERT_EQ(DatePeriod::Yesterday, period);
}

TEST(DateTimeUtilsSuite, testGetPeriodThisWeek)
{
    const DateInterval interval("20140901", "20140907");

    DatePeriod::Period period = GetDatePeriod(interval, "20140904");
    ASSERT_EQ(DatePeriod::ThisWeek, period);
}

TEST(DateTimeUtilsSuite, testGetPeriodLastWeek)
{
    const DateInterval interval("20140825", "20140831");

    DatePeriod::Period period = GetDatePeriod(interval, "20140904");
    ASSERT_EQ(DatePeriod::LastWeek, period);
}

TEST(DateTimeUtilsSuite, testGetPeriodThisMonth)
{
    const DateInterval interval("20140901", "20140930");

    DatePeriod::Period period = GetDatePeriod(interval, "20140904");
    ASSERT_EQ(DatePeriod::ThisMonth, period);
}

TEST(DateTimeUtilsSuite, testGetPeriodLastMonth)
{
    const DateInterval interval("20140801", "20140831");

    DatePeriod::Period period = GetDatePeriod(interval, "20140904");
    ASSERT_EQ(DatePeriod::LastMonth, period);
}

TEST(DateTimeUtilsSuite, testGetPeriodThisQuarter)
{
    const DateInterval interval("20140701", "20140930");

    DatePeriod::Period period = GetDatePeriod(interval, "20140904");
    ASSERT_EQ(DatePeriod::ThisQuarter, period);
}

TEST(DateTimeUtilsSuite, testGetPeriodLastQuarter)
{
    const DateInterval interval("20140401", "20140630");

    DatePeriod::Period period = GetDatePeriod(interval, "20140904");
    ASSERT_EQ(DatePeriod::LastQuarter, period);
}

TEST(DateTimeUtilsSuite, testGetPeriodThisYear)
{
    const DateInterval interval("20140101", "20141231");

    DatePeriod::Period period = GetDatePeriod(interval, "20140904");
    ASSERT_EQ(DatePeriod::ThisYear, period);
}

TEST(DateTimeUtilsSuite, testGetPeriodLastYear)
{
    const DateInterval interval("20130101", "20131231");

    DatePeriod::Period period = GetDatePeriod(interval, "20140904");
    ASSERT_EQ(DatePeriod::LastYear, period);
}

TEST(DateTimeUtilsSuite, testGetPeriodByDates)
{
    const DateInterval interval("20140901", "20140910");

    DatePeriod::Period period = GetDatePeriod(interval, "20140904");
    ASSERT_EQ(DatePeriod::ByDates, period);
}


