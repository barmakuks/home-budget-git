#pragma once
#include <string>
#include <boost/lexical_cast.hpp>
#include <memory>

namespace hb
{
namespace storage
{
class IFieldDescription
{
public:
    IFieldDescription(const char* name)
        : m_name(name)
    {
    }

    std::string Name() const
    {
        return m_name;
    }
    virtual std::string Value() const = 0;

private:
    const std::string m_name;
};

typedef std::shared_ptr<IFieldDescription> FieldDescriptionPtr;
typedef std::vector<FieldDescriptionPtr> FieldDescriptionList;

template <typename T>
class FieldDescription : public IFieldDescription
{
public:
    FieldDescription(const char* name, T value)
        : IFieldDescription(name)
        , m_value(value)
    {
    }
    virtual std::string Value() const
    {
        std::stringstream ss;
        ss << m_value;

        return ss.str();
    }

private:
    T m_value;
};

template <>
class FieldDescription<std::string> : public IFieldDescription
{
public:
    FieldDescription(const char* name, std::string value)
        : IFieldDescription(name)
        , m_value(value)
    {
    }
    virtual std::string Value() const
    {
        if (m_value.empty())
        {
            return "NULL";
        }
        else
        {
            return "'" + m_value + "'";
        }
    }

private:
    std::string m_value;
};

template <>
class FieldDescription<const char*> : public IFieldDescription
{
public:
    FieldDescription(const char* name, const char* value)
        : IFieldDescription(name)
        , m_value(value)
    {
    }
    std::string Value() const
    {
        if (m_value && !*m_value)
        {
            return "NULL";
        }

        std::string result = "'";
        result += m_value;
        result += "'";
        return result;
    }

private:
    const char* m_value;
};

template <typename T>
FieldDescriptionPtr CreateFieldDescription(const char* name, T value)
{
    return FieldDescriptionPtr(new FieldDescription<T>(name, value));
}

}  // namespace storage
}  // namespace hb
