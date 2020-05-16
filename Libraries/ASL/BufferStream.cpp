#include "BufferStream.h"

namespace ASL {

BufferStream& BufferStream::operator<<(i8 value)
{
    if (m_allow_growth)
        m_buffer->ensure_capacity(m_offset + sizeof(value));

    at(m_offset++) = value;
    return *this;
}

const BufferStream& BufferStream::operator>>(i8& value) const
{
    if (m_offset + sizeof(value) > m_buffer->size()) {
        m_read_failure = true;
        return *this;
    }

    value = at(m_offset++);
    return *this;
}

BufferStream& BufferStream::operator<<(u8 value)
{
    if (m_allow_growth)
        m_buffer->ensure_capacity(m_offset + sizeof(value));

    at(m_offset++) = value;
    return *this;
}

const BufferStream& BufferStream::operator>>(u8& value) const
{
    if (m_offset + sizeof(value) > m_buffer->size()) {
        m_read_failure = true;
        return *this;
    }

    value = at(m_offset++);
    return *this;
}

BufferStream& BufferStream::operator<<(bool value)
{
    if (m_allow_growth)
        m_buffer->ensure_capacity(m_offset + sizeof(value));

    at(m_offset++) = value;
    return *this;
}

const BufferStream& BufferStream::operator>>(bool& value) const
{
    if (m_offset + sizeof(value) > m_buffer->size()) {
        m_read_failure = true;
        return *this;
    }

    value = at(m_offset++);
    return *this;
}

BufferStream& BufferStream::operator<<(float value)
{
    if (m_allow_growth)
        m_buffer->ensure_capacity(m_offset + sizeof(value));

    union bits {
        float as_float;
        u32 as_u32;
    } u;

    u.as_float = value;
    return *this << u.as_u32;
}

const BufferStream& BufferStream::operator>>(float& value) const
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

BufferStream& BufferStream::operator<<(u16 value)
{
    if (m_allow_growth)
        m_buffer->ensure_capacity(m_offset + sizeof(value));

    value = htobe16(value);
    m_buffer->overwrite(&value, sizeof(value), m_offset);
    m_offset += sizeof(value);
    return *this;
}

const BufferStream& BufferStream::operator>>(u16& value) const
{
    if (m_offset + sizeof(value) > m_buffer->size()) {
        m_read_failure = true;
        return *this;
    }

    value = 0;
    m_buffer->read(&value, sizeof(value), m_offset);
    value = be16toh(value);
    m_offset += sizeof(value);

    return *this;
}

BufferStream& BufferStream::operator<<(i16 value)
{
    if (m_allow_growth)
        m_buffer->ensure_capacity(m_offset + sizeof(value));

    value = htobe16(value);
    m_buffer->overwrite(&value, sizeof(value), m_offset);
    m_offset += sizeof(value);
    return *this;
}

const BufferStream& BufferStream::operator>>(i16& value) const
{
    if (m_offset + sizeof(value) > m_buffer->size()) {
        m_read_failure = true;
        return *this;
    }

    value = 0;
    m_buffer->read(&value, sizeof(value), m_offset);
    value = be16toh(value);
    m_offset += sizeof(value);

    return *this;
}

BufferStream& BufferStream::operator<<(u32 value)
{
    if (m_allow_growth)
        m_buffer->ensure_capacity(m_offset + sizeof(value));

    value = htobe32(value);
    m_buffer->overwrite(&value, sizeof(value), m_offset);
    m_offset += sizeof(value);
    return *this;
}

const BufferStream& BufferStream::operator>>(u32& value) const
{
    if (m_offset + sizeof(value) > m_buffer->size()) {
        m_read_failure = true;
        return *this;
    }

    value = 0;
    m_buffer->read(&value, sizeof(value), m_offset);
    value = be32toh(value);
    m_offset += sizeof(value);

    return *this;
}

BufferStream& BufferStream::operator<<(i32 value)
{
    if (m_allow_growth)
        m_buffer->ensure_capacity(m_offset + sizeof(value));

    value = htobe32(value);
    m_buffer->overwrite(&value, sizeof(value), m_offset);
    m_offset += sizeof(value);
    return *this;
}

const BufferStream& BufferStream::operator>>(i32& value) const
{
    if (m_offset + sizeof(value) > m_buffer->size()) {
        m_read_failure = true;
        return *this;
    }

    value = 0;
    m_buffer->read(&value, sizeof(value), m_offset);
    value = be32toh(value);
    m_offset += sizeof(value);

    return *this;
}

BufferStream& BufferStream::operator<<(u64 value)
{
    if (m_allow_growth)
        m_buffer->ensure_capacity(m_offset + sizeof(value));

    value = htobe64(value);
    m_buffer->overwrite(&value, sizeof(value), m_offset);
    m_offset += sizeof(value);
    return *this;
}

const BufferStream& BufferStream::operator>>(u64& value) const
{
    if (m_offset + sizeof(value) > m_buffer->size()) {
        m_read_failure = true;
        return *this;
    }

    value = 0;
    m_buffer->read(&value, sizeof(value), m_offset);
    value = be64toh(value);
    m_offset += sizeof(value);

    return *this;
}

BufferStream& BufferStream::operator<<(i64 value)
{
    if (m_allow_growth)
        m_buffer->ensure_capacity(m_offset + sizeof(value));

    value = htobe64(value);
    m_buffer->overwrite(&value, sizeof(value), m_offset);
    m_offset += sizeof(value);
    return *this;
}

const BufferStream& BufferStream::operator>>(i64& value) const
{
    if (m_offset + sizeof(value) > m_buffer->size()) {
        m_read_failure = true;
        return *this;
    }

    value = 0;
    m_buffer->read(&value, sizeof(value), m_offset);
    value = be64toh(value);
    m_offset += sizeof(value);

    return *this;
}

BufferStream& BufferStream::operator<<(const char* value)
{
    return *this << StringView(value);
}

BufferStream& BufferStream::operator<<(const StringView& value)
{
    if (m_allow_growth)
        m_buffer->ensure_capacity(m_offset + value.length());

    for (size_t i = 0; i < value.length(); ++i)
        at(m_offset++) = value[i];
    return *this;
}

BufferStream& BufferStream::operator<<(const ByteBuffer& value)
{
    if (m_allow_growth)
        m_buffer->ensure_capacity(m_offset + value.size());

    for (size_t i = 0; i < value.size(); ++i)
        at(m_offset++) = value[i];
    return *this;
}

const BufferStream& BufferStream::operator>>(String& str) const
{
    if (m_offset >= m_buffer->size()) {
        m_read_failure = true;
        return *this;
    }

    size_t string_size = 0;
    while (m_offset + string_size < m_buffer->size() && at(m_offset + string_size)) {
        ++string_size;
    }

    str = String(reinterpret_cast<const char*>(&at(m_offset)), string_size);
    m_offset += string_size + 1;

    return *this;
}

void BufferStream::read_raw(u8* raw_data, size_t size) const
{
    if (m_offset + size > m_buffer->size()) {
        m_read_failure = true;
        return;
    }

    m_buffer->read(raw_data, size, m_offset);
    m_offset += size;
};

void BufferStream::write_raw(const u8* raw_data, size_t size)
{
    if (m_allow_growth)
        m_buffer->ensure_capacity(m_offset + size);

    m_buffer->overwrite(raw_data, size, m_offset);
    m_offset += size;
}

} // namespace ASL