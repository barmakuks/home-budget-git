#pragma once

#include "fill-strategy.hpp"
#include "report-item.h"
#include "set-value-functions.h"

namespace hb
{
namespace sql_storage
{
class FillReportStrategy : public IDatabaseEngine::ICallbackStrategy
{
public:
    using ResultType = ReportPtr;

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
    ResultType m_report;
    ReportItemPtr m_report_item;
    ReportAmountItemPtr m_amount;

    // ICallbackStrategy interface
public:
};

}  // namespace sql_storage
}  // namespace hb
