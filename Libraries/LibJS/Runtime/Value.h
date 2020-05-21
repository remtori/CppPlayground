#pragma once

#include "../Forward.h"
#include <ASL/RefCounted.h>
#include <ASL/SharedString.h>
#include <ASL/StringImpl.h>
#include <ASL/Types.h>
#include <math.h>

namespace JS {

class Value : public RefCounted<Value> {
public:
    enum class Type {
        Undefined,
        Null,
        Number,
        Bool,
        String,
        Object,
    };

    Value(Type type)
        : m_type(type)
    {
    }

    explicit Value(double value);
    explicit Value(bool value);
    Value(const SharedString& string);

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

    Object as_object() const
    {
        ASSERT(is_object());
        return *m_value.as_object;
    }

    Value to_primitive();
    bool to_bool();
    Value to_number();
    i32 to_i32();
    u32 to_u32();
    SharedString to_string();
    Object to_object();

    ~Value();

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

Value js_undefined()
{
    return Value(Value::Type::Undefined);
}

Value js_null()
{
    return Value(Value::Type::Null);
}

Value js_nan()
{
    return Value(::nan(""));
}

Value js_positive_infinity()
{
    return Value(INFINITY);
}

Value js_negative_infinity()
{
    return Value(-INFINITY);
}

} // namespace JS
