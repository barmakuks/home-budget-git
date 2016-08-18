#pragma once
#include <map>
#include <vector>
#include <type_traits>

//****** IS VECTOR *********************************************************************//
template <typename T>
struct is_vector
{
    static const bool value = std::is_base_of<std::vector<typename T::value_type>, T>::value;
};

template <typename K>
struct is_vector<std::vector<K> >
{
    static const bool value = true;
};

//****** IS MAP   *********************************************************************//

template <typename T, bool isVector>
struct is_map_helper
{
    static const bool value
        = std::is_base_of<std::map<typename T::key_type, typename T::mapped_type>, T>::value;
};

template <typename T>
struct is_map_helper<T, true>
{
    static const bool value = false;
};

template <typename T>
struct is_map
{
    static const bool value = is_map_helper<T, is_vector<T>::value>::value;
};

template <typename K, typename V>
struct is_map<std::map<K, V> >
{
    static const bool value = true;
};

//***************************************************************************//

template <typename T, bool isMap>
struct extract_map_row_type
{
    typedef T type;
};

template <typename T>
struct extract_map_row_type<T, true>  // T is a map
{
    typedef typename T::value_type type;
};

template <typename T, bool isVector>
struct extract_collection_row_type
{
    typedef typename extract_map_row_type<T, is_map<T>::value>::type type;
};

template <typename T>
struct extract_collection_row_type<T, true>  // T is a vector
{
    typedef typename T::value_type type;
};

//****** collection row type
//************************************************************************//
template <typename T>
struct get_collection_row_type
{
    typedef typename extract_collection_row_type<T, is_vector<T>::value>::type type;
};

//****** item type
//**********************************************************************************//
template <typename T, bool isMap>
struct extract_map_item_type
{
    typedef T type;
};

template <typename T>
struct extract_map_item_type<T, true>
{
    typedef typename T::mapped_type type;
};

template <typename T, bool isVector>
struct extract_collection_item_type
{
    typedef typename extract_map_item_type<T, is_map<T>::value>::type type;
};

template <typename T>
struct extract_collection_item_type<T, true>
{
    typedef typename T::value_type type;
};

template <typename T>
struct get_collection_item_type
{
    typedef typename extract_collection_item_type<T, is_vector<T>::value>::type type;
};

//***************************************************************************************************//
