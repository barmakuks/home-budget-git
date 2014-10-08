#ifndef FILLACCOUNTSMAPSTRATEGY_H
#define FILLACCOUNTSMAPSTRATEGY_H

#include "idatabase-engine.h"
#include "account.h"
namespace hb
{
namespace storage
{

class FillAccountsMapStrategy: public hb::core::IDatabaseEngine::ICallbackStrategy
{
public:
    typedef core::AccountMapPtr ResultType;

    FillAccountsMapStrategy();

    virtual void NewRecord();

    virtual void ApplyRecord();

    virtual void AddColumnValue(const std::string& fieldName, const std::string& value);

    const ResultType& Result() const
    {
        return m_accounts;
    }

private:
    typedef ResultType::element_type    map_type;
    typedef map_type::mapped_type       record_type;
    typedef record_type::element_type   account_type;

    ResultType  m_accounts;
    record_type m_account;
};

void SetValue(hb::core::Account& account, const std::string& field, const std::string& value);

} // namespace storage
} // namespace hb

#endif // FILLACCOUNTSMAPSTRATEGY_H
