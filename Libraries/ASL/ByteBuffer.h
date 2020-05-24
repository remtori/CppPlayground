#pragma once

#include <ASL/Assertions.h>
#include <ASL/Forward.h>
#include <ASL/Types.h>

namespace ASL {

class LIB_API ByteBuffer {
public:
    static ByteBuffer from_data(const char*);
    static ByteBuffer from_data(const void* ptr, size_t byte_size);

    ByteBuffer() {}
    ByteBuffer(size_t inital_capacity)
    {
        ensure_capacity(inital_capacity);
    }

    ByteBuffer(const ByteBuffer&);
    ByteBuffer(ByteBuffer&&);
    ~ByteBuffer();

    void clear();

    size_t size() const { return m_size; }
    u8* data() { return m_buffer; }
    const u8* data() const { return m_buffer; }

    u8& operator[](size_t index)
    {
        ASSERT(index < m_size);
        return m_buffer[index];
    }

    const u8& operator[](size_t index) const
    {
        ASSERT(index < m_size);
        return m_buffer[index];
    }

    void read(void* other, size_t byte_size, size_t offset = 0) const;
    bool sub_equals(const void* other, size_t byte_size, size_t offset = 0) const;

    void append(const void* data, size_t byte_size);
    void overwrite(const void* data, size_t byte_size, size_t offset = 0);
    void fill(u8 c, size_t byte_size = 0, size_t offset = 0);

    void grow(size_t growth_size, bool fill_null_space = true);
    void ensure_capacity(size_t new_capacity, bool fill_null_space = true);

    // Note: Trim do not reallocate
    void trim(size_t size)
    {
        ASSERT(size <= m_size);
        m_size = size;
    }

private:
    size_t m_size = 0;
    u8* m_buffer = nullptr;
};

}; // namespace ASL

using ASL::ByteBuffer;
