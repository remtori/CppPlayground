#pragma once

#include "Forward.h"
#include "String.h"
#include "Types.h"

namespace ASL {

class LIB_API JsonValue {
public:
    enum class Type {
        Undefined,
        Null,
        Int32,
        UInt32,
        Int64,
        UInt64,
        Double,
        Bool,
        String,
        Array,
        Object,
    };

    explicit JsonValue(Type = Type::Null);
    ~JsonValue() { clear(); }

    JsonValue(const JsonValue&);
    JsonValue(JsonValue&&);

    JsonValue& operator=(const JsonValue&);
    JsonValue& operator=(JsonValue&&);

    JsonValue(i32);
    JsonValue(u32);
    JsonValue(i64);
    JsonValue(u64);
    JsonValue(double);
    JsonValue(bool);
    JsonValue(const char*);
    JsonValue(const String&);
    JsonValue(const JsonArray&);
    JsonValue(const JsonObject&);
    JsonValue(JsonArray&&);
    JsonValue(JsonObject&&);

    Type type() const
    {
        return m_type;
    }

    bool is_null() const { return m_type == Type::Null; }
    bool is_undefined() const { return m_type == Type::Undefined; }
    bool is_bool() const { return m_type == Type::Bool; }
    bool is_string() const { return m_type == Type::String; }
    bool is_i32() const { return m_type == Type::Int32; }
    bool is_u32() const { return m_type == Type::UInt32; }
    bool is_i64() const { return m_type == Type::Int64; }
    bool is_u64() const { return m_type == Type::UInt64; }
    bool is_double() const { return m_type == Type::Double; }
    bool is_array() const { return m_type == Type::Array; }
    bool is_object() const { return m_type == Type::Object; }

    bool is_number() const
    {
        switch (m_type) {
        case Type::Int32:
        case Type::UInt32:
        case Type::Int64:
        case Type::UInt64:
        case Type::Double:
            return true;
        default:
            return false;
        }
    }

    i32 as_i32() const
    {
        ASSERT(is_i32());
        return m_value.as_i32;
    }

    u32 as_u32() const
    {
        ASSERT(is_u32());
        return m_value.as_u32;
    }

    i64 as_i64() const
    {
        ASSERT(is_i64());
        return m_value.as_i64;
    }

    u64 as_u64() const
    {
        ASSERT(is_u64());
        return m_value.as_u64;
    }

    int as_bool() const
    {
        ASSERT(is_bool());
        return m_value.as_bool;
    }

    String as_string() const
    {
        ASSERT(is_string());
        return *m_value.as_string;
    }

    const JsonObject& as_object() const
    {
        ASSERT(is_object());
        return *m_value.as_object;
    }

    const JsonArray& as_array() const
    {
        ASSERT(is_array());
        return *m_value.as_array;
    }

    double as_double() const
    {
        ASSERT(is_double());
        return m_value.as_double;
    }

    template<typename T>
    T to_number(T default_value = 0) const
    {
        if (is_double())
            return (T)as_double();
        if (type() == Type::Int32)
            return (T)as_i32();
        if (type() == Type::UInt32)
            return (T)as_u32();
        if (type() == Type::Int64)
            return (T)as_i64();
        if (type() == Type::UInt64)
            return (T)as_u64();
        return default_value;
    }

    bool equals(const JsonValue& other) const;

    void serialize(StringBuilder&, int space = 0, int indent = 0) const;
    String to_string(int space = 0) const;

private:
    void clear();
    void copy_from(const JsonValue& other);

    Type m_type = Type::Undefined;

    union {
        StringImpl* as_string = nullptr;
        JsonArray* as_array;
        JsonObject* as_object;
        double as_double;
        i32 as_i32;
        u32 as_u32;
        i64 as_i64;
        u64 as_u64;
        bool as_bool;
    } m_value;
};

}; // namespace ASL

using ASL::JsonValue;
