#pragma once
#include "raw-types.h"

namespace hb
{
namespace chrono
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

} // namespace chrono
} // namespace hb
