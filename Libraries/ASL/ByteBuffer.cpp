#include "ByteBuffer.h"

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
    buffer.copy_from(ptr, byte_size);
    return buffer;
}

ByteBuffer::ByteBuffer(const ByteBuffer& other)
    : m_capacity(other.m_capacity)
{
    ensure_capacity(m_capacity);
    memcpy(m_buffer, other.m_buffer, m_capacity);
}

ByteBuffer::ByteBuffer(ByteBuffer&& other)
    : m_capacity(other.m_capacity)
    , m_buffer(other.m_buffer)
{
    m_buffer = nullptr;
    m_capacity = 0;
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
    m_capacity = 0;
}

void ByteBuffer::copy_from(const void* ptr, size_t byte_size, size_t offset)
{
    memcpy(&m_buffer[offset], ptr, byte_size);
}

void ByteBuffer::fill(u8 c, size_t length, size_t offset)
{
    memset(&m_buffer[offset], c, length ? length : m_capacity);
}

void ByteBuffer::grow_capacity(size_t growth_size, bool fill_null_space)
{
    ensure_capacity(m_capacity + growth_size, fill_null_space);
}

void ByteBuffer::ensure_capacity(size_t new_capacity, bool fill_null_space)
{
    if (new_capacity < m_capacity)
        return;

    u8* new_buffer = (u8*)malloc(new_capacity);
    memcpy(new_buffer, m_buffer, m_capacity);

    if (fill_null_space)
        memset(&new_buffer[m_capacity], '\0', new_capacity - m_capacity);

    clear();
    m_buffer = new_buffer;
    m_capacity = new_capacity;
}

} // namespace ASL
