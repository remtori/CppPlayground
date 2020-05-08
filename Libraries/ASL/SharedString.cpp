#include "SharedString.h"

#include "HashTable.h"
#include "String.h"
#include "StringView.h"
#include "Traits.h"
#include <string.h>

namespace ASL {

struct SharedStringImplTraits : public Traits<StringImpl*> {

    static unsigned hash(const StringImpl* s) { return s ? s->hash() : 0; }

    static bool equals(const StringImpl* a, const StringImpl* b)
    {
        ASSERT(a);
        ASSERT(b);

        if (a == b)
            return true;
        if (a->length() != b->length())
            return false;

        return !memcmp(a->characters(), b->characters(), a->length());
    }
};

static HashTable<StringImpl*, SharedStringImplTraits>& shared_impls()
{
    static HashTable<StringImpl*, SharedStringImplTraits>* table;
    if (!table)
        table = new HashTable<StringImpl*, SharedStringImplTraits>;

    return *table;
}

void SharedString::will_destroy_impl(StringImpl* ptr)
{
    shared_impls().remove(ptr);
}

SharedString::SharedString(const String& string)
{
    if (string.is_null())
        return;

    if (string.impl()->is_shared()) {
        m_impl = string.impl();
        return;
    }

    auto it = shared_impls().find(const_cast<StringImpl*>(string.impl()));

    if (it == shared_impls().end()) {
        shared_impls().set(const_cast<StringImpl*>(string.impl()));
        string.impl()->set_shared(true);
        m_impl = string.impl();
    } else {
        ASSERT((*it)->is_shared());
        m_impl = *it;
    }
}

SharedString::SharedString(const StringView& string)
    : SharedString(static_cast<String>(string))
{
}

SharedString::SharedString(const char* string)
    : SharedString(static_cast<String>(string))
{
}

StringView SharedString::view() const
{
    return { characters(), length() };
}

bool SharedString::operator==(const String& string) const
{
    if (m_impl == string.impl())
        return true;
    return String(m_impl.ptr()) == string;
}

bool SharedString::operator==(const StringView& string) const
{
    return String(string) == String(m_impl.ptr());
}

bool SharedString::operator==(const char* string) const
{
    if (is_null())
        return !string;
    if (!string)
        return false;
    return !strcmp(m_impl->characters(), string);
}

} // namespace ASL