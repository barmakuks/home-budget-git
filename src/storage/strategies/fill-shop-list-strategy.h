#ifndef FILLSHOPLISTSTRATEGY_H
#define FILLSHOPLISTSTRATEGY_H


#include "idatabase-engine.h"
#include "document.h"

namespace hb
{
namespace storage
{

class FillShopListStrategy: public hb::core::IDatabaseEngine::ICallbackStrategy
{
public:
    typedef core::ShopListPtr ResultType;

    FillShopListStrategy();

    virtual void NewRecord();

    virtual void ApplyRecord();

    virtual void AddColumnValue(const std::string& fieldName, const std::string& value);

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
#endif // FILLSHOPLISTSTRATEGY_H
