#include "Value.h"

#include "Object.h"
#include <ASL/LogStream.h>
#include <ASL/StdLibExtras.h>
#include <ASL/String.h>
#include <ASL/StringBuilder.h>

namespace JS {

Value::Value(double value)
    : m_type(Type::Number)
{
    m_value.as_number = value;
}

Value::Value(bool value)
    : m_type(Type::Bool)
{
    m_value.as_bool = value;
}

Value::Value(i32 value)
    : m_type(Type::Number)
{
    m_value.as_number = value;
}

Value::Value(u32 value)
    : m_type(Type::Number)
{
    m_value.as_number = value;
}

Value::Value(const SharedString& string)
{
    if (string.is_null()) {
        m_type = Type::Null;
        return;
    }

    m_type = Type::String;
    m_value.as_string = const_cast<StringImpl*>(string.impl());
    m_value.as_string->ref();
}

Value::~Value()
{
    clear();
}

Object Value::as_object() const
{
    ASSERT(is_object());
    return *m_value.as_object;
}

Value Value::to_primitive(PreferredType preferred_type) const
{
    // TODO: Implement this properly
    if (is_object())
        return Value("[object Object]");

    return Value(*this);
}

bool Value::to_bool() const
{
    if (is_bool())
        return m_value.as_bool;

    if (is_string())
        return m_value.as_string->length() > 0;

    // TODO: NaN is false value
    if (is_number())
        return m_value.as_number != 0.0;

    if (is_object())
        return true;

    // undefined and null is false boolean
    return false;
}

Value Value::to_number() const
{
    if (is_undefined())
        return js_nan();

    if (is_null())
        return Value(+0.0);

    if (is_bool())
        return Value(as_bool() ? 1.0 : 0.0);

    if (is_number())
        return Value(*this);

    // TODO: This is wrong, fix it
    // specs: https://www.ecma-international.org/ecma-262/5.1/#sec-9.3.1
    if (is_string())
        return js_nan();

    // is_object()
    // TODO: Fix this
    return Value(+0.0);
}

i32 Value::to_i32() const
{
    auto result = to_number();
    if (result.is_nan())
        return 0;

    if (!result.is_finite_number())
        return result.as_double();

    auto number = result.as_double();

    return ASL::sign(number) * floor(abs(number));
}

u32 Value::to_u32() const
{
    return (u32)to_i32();
}

String Value::to_string() const
{
    if (is_undefined())
        return "undefined";
    if (is_null())
        return "null";
    if (is_bool())
        return as_bool() ? "true" : "false";
    if (is_string())
        return as_string();
    if (is_object())
        return as_object().to_string();

    // is_number
    // TODO: Implement
    if (is_nan())
        return "NaN";

    if (as_double() == 0.0)
        return "0";

    if (as_double() < 0) {
        StringBuilder builder;
        builder.append('-');
        builder.append(Value(-as_double()).to_string());
        return builder.to_string();
    }

    if (is_infinity())
        return "Infinity";

    return String::number(as_double());
}

Object Value::to_object() const
{
    if (is_undefined() || is_null()) {
        // TODO: Throw TypeError exception
        dbg() << "TypeError exception";
        ASSERT_NOT_REACHED();
    }

    if (is_object())
        return as_object();

    // TODO: Implement String, Number and Boolean object
    // then we can implement this
    ASSERT_NOT_REACHED();
}

bool Value::equals(const Value& other) const
{
    if (type() != other.type())
        return false;

    if (is_bool())
        return as_bool() == other.as_bool();

    if (is_string())
        return as_string() == other.as_string();

    if (is_nan() || other.is_nan())
        return false;

    if (is_number())
        return as_double() == other.as_double();

    // Compare pointer, because same object should point to the same memory
    if (is_object())
        return m_value.as_object == other.m_value.as_object;

    // The remaining type is null and undefined
    return true;
}

void Value::clear()
{
    if (m_type == Type::String) {
        m_value.as_string->unref();
    }
}

const ASL::DebugStream& operator<<(const ASL::DebugStream& stream, const Value& value)
{
    stream << value.to_string();
    return stream;
}

} // namespace JS
