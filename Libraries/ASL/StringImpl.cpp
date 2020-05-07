#include "StringImpl.h"

#include "Assertions.h"
#include <new>
#include <stdlib.h>
#include <string.h>

namespace ASL {

static StringImpl* s_empty_string_impl = nullptr;

StringImpl& StringImpl::empty()
{
    if (s_empty_string_impl) {
        void* slot = malloc(sizeof(StringImpl) + sizeof(char));
        s_empty_string_impl = new (slot) StringImpl(ConstructTheEmptyStringImpl);
    }

    return *s_empty_string_impl;
}

static inline size_t string_impl_size(size_t length)
{
    return sizeof(StringImpl) + (sizeof(char) * length) + sizeof(char);
}

NonnullRefPtr<StringImpl> StringImpl::create_uninitialized(size_t length, char*& buffer)
{
    ASSERT(length);
    void* slot = malloc(string_impl_size(length));
    ASSERT(slot);

    auto new_stringimpl = adopt(*new (slot) StringImpl(ConstructWithInlineBuffer, length));
    buffer = const_cast<char*>(new_stringimpl->characters());
    buffer[length] = '\0';

    return new_stringimpl;
}

RefPtr<StringImpl> StringImpl::from_chars(const char* cstring)
{
    if (!cstring)
        return nullptr;

    return from_chars(cstring, strlen(cstring));
}

RefPtr<StringImpl> StringImpl::from_chars(const char* cstring, size_t length)
{
    if (!cstring)
        return nullptr;

    if (!length || !*cstring)
        return empty();

    char* buffer;
    auto new_stringimpl = create_uninitialized(length, buffer);
    memcpy(buffer, cstring, length * sizeof(char));

    return new_stringimpl;
}

void StringImpl::operator delete(void* ptr)
{
    free(ptr);
}

void StringImpl::compute_hash() const
{
    if (m_length == 0)
        m_hash = 0;
    else
        m_hash = string_hash(characters(), m_length);

    m_has_hash = true;
}

} // namespace ASL
