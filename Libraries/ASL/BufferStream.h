#pragma

#include "ByteBuffer.h"
#include "NonnullOwnPtr.h"
#include "Platform.h"
#include "StdLibExtras.h"
#include "String.h"

namespace ASL {

/**
 * BufferStream to read and write data using **big endian**
 */
class BufferStream {
public:
    explicit BufferStream(ByteBuffer& buffer, bool allow_growth = false)
        : m_buffer(adopt_own(buffer))
        , m_allow_growth(allow_growth)
        , m_is_wrapper(true)
    {
    }

    BufferStream(size_t inital_size = 16, bool allow_growth = false)
        : m_buffer(create_own<ByteBuffer>(inital_size))
        , m_allow_growth(allow_growth)
        , m_is_wrapper(false)
    {
    }

    ~BufferStream()
    {
        ASSERT(!m_read_failure);
        if (m_is_wrapper)
            m_buffer.leak_ptr();
    }

    BufferStream& skip(size_t amount)
    {
        if (m_offset + amount > m_buffer->size()) {
            m_read_failure = true;
        } else {
            m_offset += amount;
        }

        return *this;
    }

    size_t offset() const { return m_offset; }

    bool has_ended() const
    {
        return m_offset == m_buffer->size();
    }

    u8 peek()
    {
        if (m_offset >= m_buffer->size()) {
            m_read_failure = true;
            return 0;
        }

        return at(m_offset);
    }

    void reset()
    {
        m_offset = 0;
        m_read_failure = false;
    }

    bool read_failed() const { return m_read_failure; }

    bool handle_read_failure()
    {
        bool old = m_read_failure;
        m_read_failure = false;
        return old;
    }

    BufferStream& operator<<(i8 value);
    BufferStream& operator>>(i8& value);
    BufferStream& operator<<(u8 value);
    BufferStream& operator>>(u8& value);
    BufferStream& operator<<(bool value);
    BufferStream& operator>>(bool& value);
    BufferStream& operator<<(float value);
    BufferStream& operator>>(float& value);
    BufferStream& operator<<(u16 value);
    BufferStream& operator>>(u16& value);
    BufferStream& operator<<(i16 value);
    BufferStream& operator>>(i16& value);
    BufferStream& operator<<(u32 value);
    BufferStream& operator>>(u32& value);
    BufferStream& operator<<(i32 value);
    BufferStream& operator>>(i32& value);
    BufferStream& operator<<(u64 value);
    BufferStream& operator>>(u64& value);
    BufferStream& operator<<(i64 value);
    BufferStream& operator>>(i64& value);
    BufferStream& operator<<(const char* value);
    BufferStream& operator<<(const StringView& value);
    BufferStream& operator<<(const ByteBuffer& value);
    BufferStream& read_raw(u8* raw_data, size_t size);
    BufferStream& operator>>(String& str);

private:
    inline u8& at(size_t offset) { return (*m_buffer)[offset]; }
    inline const u8& at(size_t offset) const { return (*m_buffer)[offset]; }

    NonnullOwnPtr<ByteBuffer> m_buffer;
    size_t m_offset = 0;
    bool m_read_failure = false;
    bool m_allow_growth = false;
    bool m_is_wrapper = false;
};

} // namespace ASL

using ASL::BufferStream;
