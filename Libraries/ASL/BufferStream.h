#pragma

#include "ByteBuffer.h"
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
        : m_buffer(buffer)
        , m_allow_growth(allow_growth)
    {
    }

    ~BufferStream()
    {
        ASSERT(!m_read_failure);
    }

    BufferStream& skip(size_t amount)
    {
        if (m_offset + amount > m_buffer.size()) {
            m_read_failure = true;
        } else {
            m_offset += amount;
        }

        return *this;
    }

    size_t offset() const { return m_offset; }

    bool has_ended() const
    {
        return m_offset == m_buffer.size();
    }

    u8 peek()
    {
        if (m_offset >= m_buffer.size()) {
            m_read_failure = true;
            return 0;
        }

        return m_buffer[m_offset];
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

    BufferStream& operator<<(i8 value)
    {
        m_buffer[m_offset++] = value;
        return *this;
    }

    BufferStream& operator>>(i8& value)
    {
        if (m_offset + sizeof(value) > m_buffer.size()) {
            m_read_failure = true;
            return *this;
        }

        value = m_buffer[m_offset++];
        return *this;
    }

    BufferStream& operator<<(u8 value)
    {
        m_buffer[m_offset++] = value;
        return *this;
    }

    BufferStream& operator>>(u8& value)
    {
        if (m_offset + sizeof(value) > m_buffer.size()) {
            m_read_failure = true;
            return *this;
        }

        value = m_buffer[m_offset++];
        return *this;
    }

    BufferStream& operator<<(bool value)
    {
        m_buffer[m_offset++] = value;
        return *this;
    }

    BufferStream& operator>>(bool& value)
    {
        if (m_offset + sizeof(value) > m_buffer.size()) {
            m_read_failure = true;
            return *this;
        }

        value = m_buffer[m_offset++];
        return *this;
    }

    BufferStream& operator<<(float value)
    {
        union bits {
            float as_float;
            u32 as_u32;
        } u;

        u.as_float = value;
        return *this << u.as_u32;
    }

    BufferStream& operator>>(float& value)
    {
        union bits {
            float as_float;
            u32 as_u32;
        } u;

        *this >> u.as_u32;
        if (m_read_failure)
            return *this;

        value = u.as_float;
        return *this;
    }

    BufferStream& operator<<(u16 value)
    {
        value = htobe16(value);
        m_buffer.overwrite(&value, sizeof(value), m_offset);
        m_offset += sizeof(value);
        return *this;
    }

    BufferStream& operator>>(u16& value)
    {
        if (m_offset + sizeof(value) > m_buffer.size()) {
            m_read_failure = true;
            return *this;
        }

        value = 0;
        m_buffer.read(&value, sizeof(value), m_offset);
        value = be16toh(value);
        m_offset += sizeof(value);

        return *this;
    }

    BufferStream& operator<<(i16 value)
    {
        value = htobe16(value);
        m_buffer.overwrite(&value, sizeof(value), m_offset);
        m_offset += sizeof(value);
        return *this;
    }

    BufferStream& operator>>(i16& value)
    {
        if (m_offset + sizeof(value) > m_buffer.size()) {
            m_read_failure = true;
            return *this;
        }

        value = 0;
        m_buffer.read(&value, sizeof(value), m_offset);
        value = be16toh(value);
        m_offset += sizeof(value);

        return *this;
    }

    BufferStream& operator<<(u32 value)
    {
        value = htobe32(value);
        m_buffer.overwrite(&value, sizeof(value), m_offset);
        m_offset += sizeof(value);
        return *this;
    }

    BufferStream& operator>>(u32& value)
    {
        if (m_offset + sizeof(value) > m_buffer.size()) {
            m_read_failure = true;
            return *this;
        }

        value = 0;
        m_buffer.read(&value, sizeof(value), m_offset);
        value = be32toh(value);
        m_offset += sizeof(value);

        return *this;
    }

    BufferStream& operator<<(i32 value)
    {
        value = htobe32(value);
        m_buffer.overwrite(&value, sizeof(value), m_offset);
        m_offset += sizeof(value);
        return *this;
    }

    BufferStream& operator>>(i32& value)
    {
        if (m_offset + sizeof(value) > m_buffer.size()) {
            m_read_failure = true;
            return *this;
        }

        value = 0;
        m_buffer.read(&value, sizeof(value), m_offset);
        value = be32toh(value);
        m_offset += sizeof(value);

        return *this;
    }

    BufferStream& operator<<(u64 value)
    {
        value = htobe64(value);
        m_buffer.overwrite(&value, sizeof(value), m_offset);
        m_offset += sizeof(value);
        return *this;
    }

    BufferStream& operator>>(u64& value)
    {
        if (m_offset + sizeof(value) > m_buffer.size()) {
            m_read_failure = true;
            return *this;
        }

        value = 0;
        m_buffer.read(&value, sizeof(value), m_offset);
        value = be64toh(value);
        m_offset += sizeof(value);

        return *this;
    }

    BufferStream& operator<<(i64 value)
    {
        value = htobe64(value);
        m_buffer.overwrite(&value, sizeof(value), m_offset);
        m_offset += sizeof(value);
        return *this;
    }

    BufferStream& operator>>(i64& value)
    {
        if (m_offset + sizeof(value) > m_buffer.size()) {
            m_read_failure = true;
            return *this;
        }

        value = 0;
        m_buffer.read(&value, sizeof(value), m_offset);
        value = be64toh(value);
        m_offset += sizeof(value);

        return *this;
    }

    BufferStream& operator<<(const char* value)
    {
        return *this << StringView(value);
    }

    BufferStream& operator<<(const StringView& value)
    {
        for (size_t i = 0; i < value.length(); ++i)
            m_buffer[m_offset++] = value[i];
        return *this;
    }

    BufferStream& operator<<(const ByteBuffer& value)
    {
        for (size_t i = 0; i < value.size(); ++i)
            m_buffer[m_offset++] = value[i];
        return *this;
    }

    BufferStream& read_raw(u8* raw_data, size_t size)
    {
        if (m_offset + size > m_buffer.size()) {
            m_read_failure = true;
            return *this;
        }

        memcpy(raw_data, m_buffer.data() + m_offset, size);
        m_offset += size;

        return *this;
    };

    BufferStream& operator>>(String& str)
    {
        if (m_offset >= m_buffer.size()) {
            m_read_failure = true;
            return *this;
        }

        size_t string_size = 0;
        while (m_offset + string_size < m_buffer.size() && m_buffer[m_offset + string_size]) {
            ++string_size;
        }

        str = String(reinterpret_cast<const char*>(&m_buffer[m_offset]), string_size);
        m_offset += string_size + 1;

        return *this;
    }

private:
    ByteBuffer& m_buffer;
    size_t m_offset = 0;
    bool m_read_failure = false;
    bool m_allow_growth = false;
};

} // namespace ASL

using ASL::BufferStream;
