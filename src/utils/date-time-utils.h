#ifndef DATETIMEUTILS_H
#define DATETIMEUTILS_H

#include "raw-types.h"

namespace hb
{
namespace utils
{

struct DatePeriod
{
    enum Period
    {
        Today,
        Yesterday,
        LastTwoDays,
        ThisWeek,
        LastWeek,
        ThisMonth,
        LastMonth,
        ThisQuarter,
        LastQuarter,
        ThisYear,
        LastYear,
        AllPeriod,
        ByDates
    };
};

struct DateInterval
{
    DateInterval() {}
    DateInterval(const Date& from, const Date& to):
        from(from),
        to(to)
    {}
    Date from;
    Date to;
};

Date Now();

DateInterval GetDateInterval(DatePeriod::Period period,
                             const Date& baseDate = Now());

DatePeriod::Period GetDatePeriod(const DateInterval& dateInterval,
                                 const Date& baseDate = Now());

std::string GetWeekDay(const Date& date);

enum Week {Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday};
extern const std::string WeekDays[];

} // namespace utils
} // namespace hb
#endif // DATETIMEUTILS_H
