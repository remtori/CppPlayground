#pragma once

#include <ASL/HashMap.h>
#include <ASL/JsonValue.h>
#include <ASL/String.h>

namespace ASL {

class LIB_API JsonObject {
public:
    JsonObject() {}
    ~JsonObject() {}

    JsonObject(const JsonObject& other)
        : m_members(other.m_members)
    {
    }

    JsonObject(JsonObject&& other)
        : m_members(move(other.m_members))
    {
    }

    JsonObject& operator=(const JsonObject& other)
    {
        if (this != &other)
            m_members = other.m_members;

        return *this;
    }

    JsonObject& operator=(JsonObject&& other)
    {
        if (this != &other)
            m_members = move(other.m_members);

        return *this;
    }

    int size() const { return m_members.size(); }
    bool is_empty() const { return m_members.is_empty(); }

    JsonValue get(const String& key) const
    {
        auto* value = get_ptr(key);
        return value ? *value : JsonValue(JsonValue::Type::Undefined);
    }

    JsonValue get_or(const String& key, JsonValue alternative) const
    {
        auto* value = get_ptr(key);
        return value ? *value : alternative;
    }

    const JsonValue* get_ptr(const String& key) const
    {
        auto it = m_members.find(key);
        if (it == m_members.end())
            return nullptr;

        return &(*it).value;
    }

    bool has(const String& key) const
    {
        return m_members.contains(key);
    }

    void set(const String& key, JsonValue value)
    {
        m_members.set(key, move(value));
    }

    HashMap<String, JsonValue>::IteratorType begin() { return m_members.begin(); }
    HashMap<String, JsonValue>::IteratorType end() { return m_members.end(); }
    HashMap<String, JsonValue>::ConstIteratorType begin() const { return m_members.begin(); }
    HashMap<String, JsonValue>::ConstIteratorType end() const { return m_members.end(); }

    template<typename Callback>
    void for_each(Callback callback) const
    {
        for (auto& it : m_members)
            callback(it.key, it.value);
    }

    void serialize(StringBuilder&, int space = 0, int indent = 0) const;
    String to_string(int space = 0) const;

private:
    HashMap<String, JsonValue> m_members;
};

} // namespace ASL

using ASL::JsonObject;
