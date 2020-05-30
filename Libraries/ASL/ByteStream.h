#pragma once

#include <ASL/ByteBuffer.h>
#include <ASL/Forward.h>
#include <ASL/Types.h>

namespace ASL {

/**
 * Little indian byte stream
 */
class LIB_API ByteStream {
private:
public:
    ByteStream();
    ~ByteStream();

    ByteStream& operator<<(u8 v);
    ByteStream& operator<<(u16 v);
    ByteStream& operator<<(u32 v);
    ByteStream& operator<<(u64 v);
    ByteStream& operator<<(i8 v);
    ByteStream& operator<<(i16 v);
    ByteStream& operator<<(i32 v);
    ByteStream& operator<<(i64 v);
    ByteStream& operator<<(const ByteBuffer&);
    ByteStream& operator<<(const char*);
    ByteStream& operator<<(const StringView&);

    ByteStream& operator>>(u8& v);
    ByteStream& operator>>(u16& v);
    ByteStream& operator>>(u32& v);
    ByteStream& operator>>(u64& v);
    ByteStream& operator>>(i8& v);
    ByteStream& operator>>(i16& v);
    ByteStream& operator>>(i32& v);
    ByteStream& operator>>(i64& v);

    void read(u8* buffer, size_t size);
    void write(const u8* buffer, size_t size);

    const ByteBuffer& buffer() const { return m_buffer; }
    ByteBuffer& buffer() { return m_buffer; }

    void reset()
    {
        m_index = 0;
    }

private:
    ByteBuffer m_buffer;
    size_t m_index;
};

} // namespace ASL

using ASL::ByteStream;
