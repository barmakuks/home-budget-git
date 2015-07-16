#ifndef FILLSTRATEGY_HPP
#define FILLSTRATEGY_HPP

#include <map>
#include <vector>
#include <type_traits>

#include "idatabase-engine.h"
#include "setters.hpp"
#include "get_type_traits.hpp"

namespace hb
{
namespace storage
{

template <typename T, typename V, bool isMap>
class AppendToCollection
{
public:
    static void Add(T& collection, V& item)
    {
        collection.push_back(item);
    }
};

template <typename T, typename V>
class AppendToCollection<T, V, true>
{
public:
    static void Add(T& collection, V& item)
    {
        collection.insert(typename T::value_type(item->Id(), item));
    }
};

template<class T, typename V>
void Add(T& collection, V& item)
{
    AppendToCollection<T, V, is_map<T>::value >::Add(collection, item);
//    collection.push_back(item);
}

template<class Type,
         void (*SetValue)(typename get_collection_item_type<typename Type::element_type>::type::element_type&, const std::string&, const std::string&)>
class FillStrategy:
    public hb::core::IDatabaseEngine::ICallbackStrategy
{
public:
    typedef Type ResultType;

    FillStrategy():
        m_collection_ptr(new collection_type())
    {}

    void NewRecord() override
    {
        m_item_ptr.reset(new typename item_type_ptr::element_type());
    }

    void ApplyRecord() override
    {
        if (m_item_ptr)
        {
            Add<collection_type, item_type_ptr>(*m_collection_ptr, m_item_ptr);
        }
    }

    void AddColumnValue(const std::string& fieldName,
                        const std::string& value) override
    {
        SetValue(*m_item_ptr, fieldName, value);
    }

    void Finalize() override
    {
    }

    const ResultType& Result() const
    {
        return m_collection_ptr;
    }

protected:
    // ResultType is shared_ptr to a vector or a map of shared pointer to class T
    typedef typename ResultType::element_type                           collection_type;        // vector or map
    typedef typename get_collection_row_type<collection_type>::type     collection_row_type;   // for vector is shared_ptr<T>, for map is pair<id, shared_ptr<T>
    typedef typename get_collection_item_type<collection_type>::type    item_type_ptr;          // shared_ptr<T>

    ResultType      m_collection_ptr;
    item_type_ptr   m_item_ptr;
};

} // namespace storage
} // namespace hb

#endif // FILLSTRATEGY_HPP
