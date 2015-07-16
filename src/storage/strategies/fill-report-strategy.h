#ifndef FILLREPORTSTRATEGY_H
#define FILLREPORTSTRATEGY_H

#include "fill-strategy.hpp"
#include "report-item.h"
#include "set-value-functions.h"

namespace hb
{
namespace storage
{

class FillReportStrategy: public hb::core::IDatabaseEngine::ICallbackStrategy
{
public:
    typedef core::ReportPtr ResultType;

    FillReportStrategy();

    const ResultType& Result() const
    {
        return m_report;
    }

    // ICallbackStrategy interface
public:
    void NewRecord() override;
    void ApplyRecord() override;
    void AddColumnValue(const std::string& fieldName, const std::string& value) override;
    void Finalize() override;

private:
    ResultType                  m_report;
    core::ReportItemPtr         m_report_item;
    core::ReportAmountItemPtr   m_amount;

    // ICallbackStrategy interface
public:
};

} // namespace storage
} // namespace hb

#endif // FILLREPORTSTRATEGY_H
