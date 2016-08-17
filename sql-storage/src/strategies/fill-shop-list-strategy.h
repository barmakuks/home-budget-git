#pragma once

#include "idatabase-engine.h"
#include "document.h"

namespace hb
{
namespace storage
{

class FillShopListStrategy: public hb::IDatabaseEngine::ICallbackStrategy
{
public:
    using ResultType = ShopListPtr;

    FillShopListStrategy();

    void NewRecord() override;

    void ApplyRecord() override;

    void AddColumnValue(const std::string& fieldName, const std::string& value) override;

    void Finalize() override
    {
    }

    const ResultType& Result() const
    {
        return m_shops;
    }

private:
    typedef ResultType::element_type    vector_type;
    typedef vector_type::value_type     row_type;

    ResultType  m_shops;
    row_type m_shop;
};

} // namespace storage
} // namespace hb
