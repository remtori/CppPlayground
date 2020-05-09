#pragma once

#include "HashMap.h"
#include "JsonValue.h"
#include "String.h"

namespace ASL {

class JsonObject {
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

    template<typename Callback>
    void for_each(Callback callback) const
    {
        for (auto& it : m_members)
            callback(it.key, it.value);
    }

private:
    HashMap<String, JsonValue> m_members;
};

} // namespace ASL