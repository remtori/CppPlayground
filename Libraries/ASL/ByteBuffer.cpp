#include "ByteBuffer.h"

#include "Assertions.h"
#include <stdlib.h>
#include <string.h>

namespace ASL {

ByteBuffer ByteBuffer::from_data(const char* cstring)
{
    return ByteBuffer::from_data(cstring, strlen(cstring));
}

ByteBuffer ByteBuffer::from_data(const void* ptr, size_t byte_size)
{
    ByteBuffer buffer(byte_size);
    buffer.overwrite(ptr, byte_size);
    return buffer;
}

ByteBuffer::ByteBuffer(const ByteBuffer& other)
    : m_size(other.m_size)
{
    ensure_capacity(m_size);
    memcpy(m_buffer, other.m_buffer, m_size);
}

ByteBuffer::ByteBuffer(ByteBuffer&& other)
    : m_size(other.m_size)
    , m_buffer(other.m_buffer)
{
    other.m_buffer = nullptr;
    other.m_size = 0;
}

ByteBuffer::~ByteBuffer()
{
    clear();
}

void ByteBuffer::clear()
{
    if (m_buffer)
        free(m_buffer);

    m_buffer = nullptr;
    m_size = 0;
}

void ByteBuffer::append(const void* ptr, size_t byte_size)
{
    if (byte_size == 0)
        return;

    size_t old_size = m_size;

    grow(byte_size, false);
    overwrite(ptr, byte_size, old_size);
}

void ByteBuffer::overwrite(const void* ptr, size_t byte_size, size_t offset)
{
    ASSERT(ptr != nullptr);
    ASSERT(offset + byte_size <= m_size);
    memcpy(&m_buffer[offset], ptr, byte_size);
}

void ByteBuffer::fill(u8 c, size_t byte_size, size_t offset)
{
    if (byte_size == 0)
        byte_size = m_size;

    ASSERT(offset + byte_size <= m_size);
    memset(&m_buffer[offset], c, byte_size);
}

void ByteBuffer::grow(size_t growth_size, bool fill_null_space)
{
    ensure_capacity(m_size + growth_size, fill_null_space);
}

void ByteBuffer::ensure_capacity(size_t new_capacity, bool fill_null_space)
{
    if (new_capacity < m_size)
        return;

    u8* new_buffer = (u8*)malloc(new_capacity);
    memcpy(new_buffer, m_buffer, m_size);

    if (fill_null_space)
        memset(&new_buffer[m_size], '\0', new_capacity - m_size);

    clear();
    m_buffer = new_buffer;
    m_size = new_capacity;
}

} // namespace ASL
