#include "JsonValue.h"

#include "Assertions.h"
#include "JsonArray.h"
#include "JsonObject.h"
#include "StdLibExtras.h"
#include "StringBuilder.h"
#include "StringImpl.h"

namespace ASL {

JsonValue::JsonValue(Type type)
    : m_type(type)
{
}

JsonValue::JsonValue(const JsonValue& other)
{
    copy_from(other);
}

JsonValue& JsonValue::operator=(const JsonValue& other)
{
    if (this != &other) {
        clear();
        copy_from(other);
    }
    return *this;
}

JsonValue::JsonValue(JsonValue&& other)
{
    m_type = exchange(other.m_type, Type::Undefined);
    m_value.as_string = exchange(other.m_value.as_string, nullptr);
}

JsonValue& JsonValue::operator=(JsonValue&& other)
{
    if (this != &other) {
        clear();
        m_type = exchange(other.m_type, Type::Undefined);
        m_value.as_string = exchange(other.m_value.as_string, nullptr);
    }

    return *this;
}

void JsonValue::copy_from(const JsonValue& other)
{
    m_type = other.m_type;
    switch (m_type) {
    case Type::String:
        ASSERT(!m_value.as_string);
        m_value.as_string = other.m_value.as_string;
        m_value.as_string->ref();
        break;
    case Type::Object:
        m_value.as_object = new JsonObject(*other.m_value.as_object);
        break;
    case Type::Array:
        m_value.as_array = new JsonArray(*other.m_value.as_array);
        break;
    default:
        m_value.as_string = other.m_value.as_string;
        break;
    }
}

JsonValue::JsonValue(i32 v)
    : m_type(Type::Int32)
{
    m_value.as_i32 = v;
}

JsonValue::JsonValue(u32 v)
    : m_type(Type::UInt32)
{
    m_value.as_u32 = v;
}

JsonValue::JsonValue(i64 v)
    : m_type(Type::Int64)
{
    m_value.as_i64 = v;
}

JsonValue::JsonValue(u64 v)
    : m_type(Type::UInt64)
{
    m_value.as_u64 = v;
}

JsonValue::JsonValue(double v)
    : m_type(Type::Double)
{
    m_value.as_double = v;
}

JsonValue::JsonValue(bool v)
    : m_type(Type::Bool)
{
    m_value.as_bool = v;
}

JsonValue::JsonValue(const char* cstring)
    : JsonValue(String(cstring))
{
}

JsonValue::JsonValue(const String& str)
{
    if (str.is_null()) {
        m_type = Type::Null;
    } else {
        m_type = Type::String;
        m_value.as_string = const_cast<StringImpl*>(str.impl());
        m_value.as_string->ref();
    }
}

JsonValue::JsonValue(const JsonArray& v)
    : m_type(Type::Array)
{
    m_value.as_array = new JsonArray(v);
}

JsonValue::JsonValue(const JsonObject& v)
    : m_type(Type::Object)
{
    m_value.as_object = new JsonObject(v);
}

JsonValue::JsonValue(JsonArray&& v)
    : m_type(Type::Array)
{
    m_value.as_array = new JsonArray(move(v));
}

JsonValue::JsonValue(JsonObject&& v)
    : m_type(Type::Object)
{
    m_value.as_object = new JsonObject(move(v));
}

bool JsonValue::equals(const JsonValue& other) const
{
    if (is_null() && other.is_null())
        return true;

    if (is_bool() && other.is_bool() && as_bool() == other.as_bool())
        return true;

    if (is_string() && other.is_string() && as_string() == other.as_string())
        return true;

    if (is_number() && other.is_number() && to_number<double>() == other.to_number<double>()) {
        return true;
    }

    if (is_array() && other.is_array() && as_array().size() == other.as_array().size()) {
        bool result = true;
        for (int i = 0; i < as_array().size(); ++i) {
            result &= as_array().at(i).equals(other.as_array().at(i));
        }

        return result;
    }

    if (is_object() && other.is_object() && as_object().size() == other.as_object().size()) {
        bool result = true;
        as_object().for_each([&](auto& key, auto& value) {
            result &= value.equals(other.as_object().get(key));
        });

        return result;
    }

    return false;
}

void JsonValue::clear()
{
    switch (m_type) {
    case Type::String:
        m_value.as_string->unref();
        break;
    case Type::Object:
        delete m_value.as_object;
        break;
    case Type::Array:
        delete m_value.as_array;
        break;
    default:
        break;
    }

    m_type = Type::Undefined;
    m_value.as_string = nullptr;
}

void JsonValue::serialize(StringBuilder& builder, int space, int indent) const
{
    switch (m_type) {
    case Type::Int32:
    case Type::Int64:
        builder.append(String::number(as_i32()));
        break;
    case Type::UInt32:
    case Type::UInt64:
        builder.append(String::number(as_u32()));
        break;
    case Type::Double:
        builder.append(String::number(as_double()));
        break;
    case Type::Bool:
        builder.append(as_bool() ? "true" : "false");
        break;
    case Type::String:
        builder.append('"');
        // TODO: need to escapse special chars
        builder.append(as_string());
        builder.append('"');
        break;
    case Type::Array:
        as_array().serialize(builder, space, indent);
        break;
    case Type::Object:
        as_object().serialize(builder, space, indent);
        break;
    case Type::Null:
    default:
        builder.append("null");
        break;
    }
}

String JsonValue::to_string(int space) const
{
    StringBuilder builder;
    serialize(builder, space);
    return builder.to_string();
}

} // namespace ASL