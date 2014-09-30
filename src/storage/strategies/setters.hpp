#ifndef SETTERS_HPP
#define SETTERS_HPP

#include <boost/lexical_cast.hpp>

template<typename T, class D, void (D::*Setter)(T)>
class FieldSetter
{
    typedef std::shared_ptr<D> type_ptr;
public:
    static void SetValue(type_ptr& doc,
                         const std::string& expectedField,
                         const std::string& field, const std::string& value)
    {
        if (doc && field == expectedField && !value.empty())
        {
            try
            {
                long long id = boost::lexical_cast<long long>(value);
                ((*doc).*Setter)(static_cast<T>(id));
            }
            catch(boost::bad_lexical_cast const& )
            {
                doc.reset();
                return;
            }
        }
    }
};

template<typename D, void (D::*Setter)(const std::string&)>
class FieldSetter<const std::string&, D, Setter>
{
    typedef std::shared_ptr<D> type_ptr;
public:
    static void SetValue(type_ptr& doc,
                         const std::string& expectedField,
                         const std::string& field, const std::string& value)
    {
        if (doc && field == expectedField)
        {
            ((*doc).*Setter)(value);
        }
    }
};

#endif // SETTERS_HPP
