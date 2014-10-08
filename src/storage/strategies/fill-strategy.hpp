#ifndef FILLSTRATEGY_HPP
#define FILLSTRATEGY_HPP

#include <map>
#include <vector>

#include "idatabase-engine.h"
#include "setters.hpp"

namespace hb
{
namespace storage
{

template <typename T>
struct get_item_type{ typedef T type;};

template <typename K, typename V>
struct get_item_type<std::map<K,V> > {typedef typename std::map<K,V>::mapped_type type;};

template <typename T>
struct get_item_type<std::vector<T> > {typedef typename std::vector<T>::value_type type;};


template <typename T>
struct get_collection_item_type{ typedef T type;};

template <typename K, typename V>
struct get_collection_item_type<std::map<K,V> > {typedef typename std::map<K,V>::value_type type;};

template <typename T>
struct get_collection_item_type<std::vector<T> > {typedef typename std::vector<T>::value_type type;};


template <typename T, typename I>
class AppendToCollection
{
public:
    void Add(T& collection, I& item)
    {
        collection.push_back(item);
    }
};


template<class Type>
class FillStrategy:
        public hb::core::IDatabaseEngine::ICallbackStrategy
{
public:
    typedef Type ResultType;

    FillStrategy():
        m_collection(new collection_type())
    {}

    virtual void NewRecord()
    {
//        m_item.reset();
        m_item.reset(new typename item_type::element_type());
    }

    virtual void ApplyRecord()
    {
        if (m_item)
        {
            AppendToCollection<collection_type, item_type> appender;
            appender.Add(*m_collection, m_item);
        }
    }

    virtual void AddColumnValue(const std::string& fieldName,
                                const std::string& value)
    {
        SetValue(*m_item, fieldName, value);
    }

    const ResultType& Result() const
    {
        return m_collection;
    }

private:
    typedef typename ResultType::element_type                           collection_type;
    typedef typename collection_type::value_type                        collection_item_type;
    typedef typename collection_type::value_type                        item_type;
//    typedef typename get_collection_item_type<collection_type>::type    collection_item_type;
//    typedef typename get_item_type<collection_type>::type               item_type;

    ResultType  m_collection;
    item_type   m_item;
};

} // namespace storage
} // namespace hb

#endif // FILLSTRATEGY_HPP
