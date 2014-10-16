#include "date-time-utils.h"
#include <chrono>
#include <ctime>
#include <time.h>
#include <memory.h>

namespace hb
{
namespace utils
{

Date RawTimeToDate(const std::time_t& rawtime)
{
    struct tm* timeinfo;
    timeinfo = localtime (&rawtime);

    char buf[9];

    std::strftime(buf, 9, "%Y%m%d", timeinfo);

    return buf;
}

std::time_t DateToRawTime(const Date& date)
{
    struct tm time;

    memset(&time, 0, sizeof(time));

    strptime(date.c_str(), "%Y%m%d", &time);
    time_t t = mktime(&time);
    return t;
}

const std::string WeekDays[] = {"Нд", "Пн", "Вт", "Ср", "Чт", "Пт", "Сб"};

std::string GetWeekDay(const Date& date)
{
    std::time_t rawtime = DateToRawTime(date);

    struct tm* timeinfo;
    timeinfo = localtime (&rawtime);

    return WeekDays[timeinfo->tm_wday];
}


Date Now()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t rawtime = std::chrono::system_clock::to_time_t(now);

    return RawTimeToDate(rawtime);
}

time_t FirstMonthDay(int year, int month)
{
    struct tm    time;
    memset(&time, 0, sizeof(time));
    time.tm_year = year - 1900;
    time.tm_mon = month;
    if (time.tm_mon < 0)
    {
        time.tm_year += (month / 12 - 1);
        time.tm_mon += 12;
    }
    else if (time.tm_mon > 11)
    {
        time.tm_year += month / 12;
        time.tm_mon = time.tm_mon % 12;
    }

    time.tm_mday = 1;

    return mktime(&time);
}

time_t LastMonthDay(int year, int month)
{
    static const int SecondsPerDay = 24 * 60 * 60;

    return FirstMonthDay(year, month + 1) - SecondsPerDay;
}


DateInterval GetDateInterval(DatePeriod::Period period,
                             const Date& baseDate)
{
    DateInterval interval;

    std::time_t rawBaseTime = DateToRawTime(baseDate);
    struct tm* timeinfo = localtime(&rawBaseTime);
    mktime(timeinfo);
    const int weekDay = (timeinfo->tm_wday + 6) % 7; // shift sunday to the end of week
    const int day = timeinfo->tm_mday;
    const int month = timeinfo->tm_mon;
    const int year = timeinfo->tm_year + 1900;
    const int SecondsPerDay = 24 * 60 * 60;

    switch (period)
    {
    case DatePeriod::Today:
    {
        interval.from = RawTimeToDate(rawBaseTime);
        interval.to = interval.from;
        break;
    }
    case DatePeriod::Yesterday:
    {
        rawBaseTime -= SecondsPerDay;
        interval.from = RawTimeToDate(rawBaseTime);
        interval.to = interval.from;
        break;
    }
    case DatePeriod::LastTwoDays:
    {
        interval.from = RawTimeToDate(rawBaseTime - SecondsPerDay);
        interval.to  = RawTimeToDate(rawBaseTime);
        break;
    }
    case DatePeriod::ThisWeek:
    {
        interval.from = RawTimeToDate(rawBaseTime - SecondsPerDay * weekDay);
        interval.to = RawTimeToDate(rawBaseTime - (weekDay - 6) * SecondsPerDay);
        break;
    }
    case DatePeriod::LastWeek:
    {
        interval.from = RawTimeToDate(rawBaseTime - SecondsPerDay * (weekDay + 7));
        interval.to = RawTimeToDate(rawBaseTime - (weekDay + 1) * SecondsPerDay);
        break;
    }
    case DatePeriod::ThisMonth:
    {
        interval.from = RawTimeToDate(rawBaseTime - SecondsPerDay * (day - 1));
        interval.to = RawTimeToDate(LastMonthDay(year, month));
        break;
    }
    case DatePeriod::LastMonth:
    {
        interval.from = RawTimeToDate(FirstMonthDay(year, month - 1));
        interval.to = RawTimeToDate(LastMonthDay(year, month - 1));
        break;
    }
    case DatePeriod::ThisQuarter:
    {
        interval.from = RawTimeToDate(FirstMonthDay(year, month - month % 3));
        interval.to = RawTimeToDate(LastMonthDay(year, month - month % 3 + 2));
        break;
    }
    case DatePeriod::LastQuarter:
    {
        interval.from = RawTimeToDate(FirstMonthDay(year, month - month % 3 - 3));
        interval.to = RawTimeToDate(LastMonthDay(year, month - month % 3 + 2 - 3));
        break;
    }
    case DatePeriod::ThisYear:
    {
        interval.from = RawTimeToDate(FirstMonthDay(year, 0));
        interval.to = RawTimeToDate(LastMonthDay(year, 11));
        break;
    }
    case DatePeriod::LastYear:
    {
        interval.from = RawTimeToDate(FirstMonthDay(year - 1, 0));
        interval.to = RawTimeToDate(LastMonthDay(year - 1, 11));
        break;
    }
    case DatePeriod::AllPeriod:
    {
        interval.from = "19990101";
        interval.to = RawTimeToDate(rawBaseTime);
        break;
    }
    default:
        break;
    }

    return interval;
}

DatePeriod::Period GetDatePeriod(const DateInterval& dateInterval,
                                 const Date& baseDate)
{
    for (int i = DatePeriod::Today;
         i != DatePeriod::ByDates;
         ++i)
    {
        DatePeriod::Period period = static_cast<DatePeriod::Period>(i);
        DateInterval result = GetDateInterval(period, baseDate);

        if (result.from == dateInterval.from && result.to == dateInterval.to)
        {
            return period;
        }
    }

    return DatePeriod::ByDates;
}

} // namespace utils
} // namespace hb
