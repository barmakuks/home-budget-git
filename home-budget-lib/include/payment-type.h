#pragma once

#include <map>
#include <vector>
#include <memory>

#include "hb-types.h"

namespace hb
{

class PaymentType
{
public:
    PaymentType();

    PaymentTypeId Id() const { return m_id; }
    void SetId(PaymentTypeId id) { m_id = id; }

    std::string Name() const { return m_name; }
    void SetName(const std::string& name) { m_name = name; }

private:
    PaymentTypeId   m_id;
    std::string     m_name;
};

typedef std::shared_ptr<PaymentType> PaymentTypePtr;

typedef std::vector<PaymentTypePtr> PaymentTypes;

class PaymentTypesMap: public std::map<PaymentTypeId, PaymentTypePtr>
{
public:
    PaymentTypes GetList() const;
};

class PaymentTypesMapPtr: public std::shared_ptr<PaymentTypesMap>
{
public:
    PaymentTypesMapPtr(){}
    PaymentTypesMapPtr(PaymentTypesMap* ptr){reset(ptr);}
};

} // namespace hb
