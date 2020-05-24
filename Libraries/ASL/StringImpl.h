#pragma once

#include <ASL/NonnullRefPtr.h>
#include <ASL/RefCounted.h>
#include <ASL/RefPtr.h>
#include <ASL/Types.h>

namespace ASL {

class LIB_API StringImpl : public RefCounted<StringImpl> {
public:
    static StringImpl& empty();
    static NonnullRefPtr<StringImpl> create_uninitialized(size_t length, char*& buffer);
    static RefPtr<StringImpl> from_chars(const char* cstring);
    static RefPtr<StringImpl> from_chars(const char* cstring, size_t length);

    ~StringImpl();
    void operator delete(void* ptr);

    size_t length() const { return m_length; }
    const char* characters() const { return &m_buffer[0]; }

    void set(size_t index, char c)
    {
        ASSERT(index < m_length);
        if (characters()[index] != c) {
            m_buffer[index] = c;
            m_has_hash = false;
        }
    }

    const char& operator[](size_t i) const
    {
        ASSERT(i < m_length);
        return characters()[i];
    }

    bool is_shared() const { return m_is_shared; }
    void set_shared(bool shared) const { m_is_shared = shared; }

    unsigned hash() const
    {
        if (!m_has_hash)
            compute_hash();

        return m_hash;
    }

    unsigned existing_hash() const
    {
        return m_hash;
    }

private:
    void compute_hash() const;

    enum ConstructTheEmptyStringImplTag {
        ConstructTheEmptyStringImpl
    };
    explicit StringImpl(ConstructTheEmptyStringImplTag)
    {
        m_buffer[0] = '\0';
    }

    enum ConstructWithInlineBufferTag {
        ConstructWithInlineBuffer
    };
    StringImpl(ConstructWithInlineBufferTag, size_t length)
        : m_length(length)
    {
    }

    size_t m_length = 0;
    mutable bool m_is_shared = false;
    mutable unsigned m_hash = 0;
    mutable bool m_has_hash = false;

    // This need to be the last member for inline buffer data
    char m_buffer[0];
};

inline constexpr u32 string_hash(const char* characters, size_t length)
{
    u32 hash = 0;
    for (size_t i = 0; i < length; ++i) {
        hash += (u32)characters[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

} // namespace ASL

using ASL::StringImpl;
