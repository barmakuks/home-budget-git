#ifndef SETTERS_HPP
#define SETTERS_HPP

#include <boost/lexical_cast.hpp>

template<typename value_type>
typename std::remove_reference<value_type>::type ParseValue(const std::string& valueStr,
                                                            bool& result)
{
    try
    {
        if (valueStr.empty())
        {
            result = false;
            return typename std::remove_reference<value_type>::type();
        }
        long long parsed = boost::lexical_cast<long long>(valueStr);
        typename std::remove_reference<value_type>::type value = static_cast<value_type>(parsed);
        result = true;
        return value;
    }
    catch(boost::bad_lexical_cast const& )
    {
        result = false;
        return typename std::remove_reference<value_type>::type();
    }
}

template<>
inline const std::string ParseValue<const std::string&>(const std::string& valueStr,
                                                        bool& result)
{
    result = true;
    return valueStr;
}

template<class Object, typename value_type>
bool SetFieldValue(Object& obj,
                   void (Object::*setter)(value_type),
                   const std::string& expectedField,
                   const std::string& field,
                   const std::string& valueStr)
{
    if (field == expectedField)
    {
        bool result = false;
        value_type value = ParseValue<value_type>(valueStr, result);

        if (result)
        {
            (obj.*setter)(value);
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}


template<class Object, typename value_type>
inline void SetFieldValue(std::shared_ptr<Object>& obj,
                          void (Object::*setter)(value_type),
                          const std::string& expectedField,
                          const std::string& field,
                          const std::string& valueStr)
{
    if (obj && field == expectedField)
    {
        bool result = false;
        value_type value = ParseValue<value_type>(valueStr, result);

        if (result)
        {
            ((*obj).*setter)(value);
        }
        else
        {
            obj.reset();
        }
    }
}

#endif // SETTERS_HPP
