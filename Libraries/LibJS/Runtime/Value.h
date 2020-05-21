#pragma once

#include "../Forward.h"
#include <ASL/Forward.h>
#include <ASL/SharedString.h>
#include <ASL/StringImpl.h>
#include <ASL/Types.h>
#include <math.h>

namespace JS {

class Value {
public:
    enum class Type {
        Undefined,
        Null,
        Number,
        Bool,
        String,
        Object,
    };

    Value() {}

    Value(Type type)
        : m_type(type)
    {
    }

    explicit Value(double value);
    explicit Value(bool value);
    Value(const SharedString& string);
    ~Value();

    Type type() const { return m_type; }

    bool is_null() const { return m_type == Type::Null; }
    bool is_undefined() const { return m_type == Type::Undefined; }
    bool is_bool() const { return m_type == Type::Bool; }
    bool is_number() const { return m_type == Type::Number; }
    bool is_string() const { return m_type == Type::String; }
    bool is_object() const { return m_type == Type::Object; }

    bool is_nan() const { return is_number() && ::isnan(as_double()); }
    bool is_infinity() const { return is_number() && ::isinf(as_double()); }
    bool is_integer() const { return is_finite_number() && (i32)as_double() == as_double(); }
    bool is_finite_number() const
    {
        if (!is_number())
            return false;

        auto number = as_double();
        return !::isnan(number) && !::isinf(number);
    }

    bool as_bool() const
    {
        ASSERT(is_bool());
        return m_value.as_bool;
    }

    double as_double() const
    {
        ASSERT(is_number());
        return m_value.as_number;
    }

    SharedString as_string() const
    {
        ASSERT(is_string());
        return m_value.as_string;
    }

    Object as_object() const;

    Value to_primitive() const;
    bool to_bool() const;
    Value to_number() const;
    i32 to_i32() const;
    u32 to_u32() const;
    String to_string() const;
    Object to_object() const;

private:
    void clear();

    Type m_type = Type::Undefined;

    union {
        bool as_bool;
        double as_number;
        StringImpl* as_string;
        Object* as_object;
    } m_value;
};

inline Value js_undefined()
{
    return Value(Value::Type::Undefined);
}

inline Value js_null()
{
    return Value(Value::Type::Null);
}

inline Value js_nan()
{
    return Value(::nan(""));
}

inline Value js_positive_infinity()
{
    return Value(INFINITY);
}

inline Value js_negative_infinity()
{
    return Value(-INFINITY);
}

const ASL::DebugStream& operator<<(const ASL::DebugStream& stream, const Value& value);

} // namespace JS
