#include <ASL/ByteStream.h>

#include <ASL/Platform.h>
#include <ASL/StringView.h>
#include <string.h>

namespace ASL {

ByteStream::ByteStream()
{
}

ByteStream::~ByteStream()
{
}

ByteStream& ByteStream::operator<<(u8 v)
{
    m_buffer.grow_capacity(m_index + sizeof(u8));
    m_buffer.overwrite(&v, sizeof(u8), m_index);
    m_index += sizeof(u8);
    return *this;
}

ByteStream& ByteStream::operator<<(u16 v)
{
    v = htole16(v);
    m_buffer.grow_capacity(m_index + sizeof(u16));
    m_buffer.overwrite(&v, sizeof(u16), m_index);
    m_index += sizeof(u16);
    return *this;
}

ByteStream& ByteStream::operator<<(u32 v)
{
    v = htole32(v);
    m_buffer.grow_capacity(m_index + sizeof(u32));
    m_buffer.overwrite(&v, sizeof(u32), m_index);
    m_index += sizeof(u32);
    return *this;
}

ByteStream& ByteStream::operator<<(u64 v)
{
    v = htole64(v);
    m_buffer.grow_capacity(m_index + sizeof(u64));
    m_buffer.overwrite(&v, sizeof(u64), m_index);
    m_index += sizeof(u64);
    return *this;
}

ByteStream& ByteStream::operator<<(i8 v)
{
    m_buffer.grow_capacity(m_index + sizeof(u8));
    m_buffer.overwrite(&v, sizeof(u8), m_index);
    m_index += sizeof(u8);
    return *this;
}

ByteStream& ByteStream::operator<<(i16 v)
{
    v = htole16(v);
    m_buffer.grow_capacity(m_index + sizeof(u16));
    m_buffer.overwrite(&v, sizeof(u16), m_index);
    m_index += sizeof(u16);
    return *this;
}

ByteStream& ByteStream::operator<<(i32 v)
{
    v = htole32(v);
    m_buffer.grow_capacity(m_index + sizeof(u32));
    m_buffer.overwrite(&v, sizeof(u32), m_index);
    m_index += sizeof(u32);
    return *this;
}

ByteStream& ByteStream::operator<<(i64 v)
{
    v = htole64(v);
    m_buffer.grow_capacity(m_index + sizeof(u64));
    m_buffer.overwrite(&v, sizeof(u64), m_index);
    m_index += sizeof(u64);
    return *this;
}

ByteStream& ByteStream::operator<<(const ByteBuffer& buffer)
{
    m_buffer.grow_capacity(m_index + buffer.size());
    m_buffer.overwrite(buffer.data(), buffer.size(), m_index);
    m_index += buffer.size();
    return *this;
}

ByteStream& ByteStream::operator<<(const char* cstring)
{
    size_t size = strlen(cstring);
    m_buffer.grow_capacity(m_index + size);
    m_buffer.overwrite(cstring, size, m_index);
    m_index += size;
    return *this;
}

ByteStream& ByteStream::operator<<(const StringView& view)
{
    m_buffer.grow_capacity(m_index + view.length());
    m_buffer.overwrite(view.characters_wont(), view.length(), m_index);
    m_index += view.length();
    return *this;
}

ByteStream& ByteStream::operator>>(u8& v)
{
    m_buffer.read(&v, sizeof(u8), m_index);
    m_index += sizeof(u8);
    return *this;
}

ByteStream& ByteStream::operator>>(u16& v)
{
    m_buffer.read(&v, sizeof(u16), m_index);
    m_index += sizeof(u16);
    v = le16toh(v);
    return *this;
}

ByteStream& ByteStream::operator>>(u32& v)
{
    m_buffer.read(&v, sizeof(u32), m_index);
    m_index += sizeof(u32);
    v = le32toh(v);
    return *this;
}

ByteStream& ByteStream::operator>>(u64& v)
{
    m_buffer.read(&v, sizeof(u64), m_index);
    m_index += sizeof(u64);
    v = le64toh(v);
    return *this;
}

ByteStream& ByteStream::operator>>(i8& v)
{
    m_buffer.read(&v, sizeof(u8), m_index);
    m_index += sizeof(u8);
    return *this;
}

ByteStream& ByteStream::operator>>(i16& v)
{
    m_buffer.read(&v, sizeof(u16), m_index);
    m_index += sizeof(u16);
    v = le16toh(v);
    return *this;
}

ByteStream& ByteStream::operator>>(i32& v)
{
    m_buffer.read(&v, sizeof(u32), m_index);
    m_index += sizeof(u32);
    v = le32toh(v);
    return *this;
}

ByteStream& ByteStream::operator>>(i64& v)
{
    m_buffer.read(&v, sizeof(u64), m_index);
    m_index += sizeof(u64);
    v = le64toh(v);
    return *this;
}

void ByteStream::read(u8* buffer, size_t size)
{
    m_buffer.read(buffer, size, m_index);
    m_index += size;
}

void ByteStream::write(const u8* buffer, size_t size)
{
    m_buffer.overwrite(buffer, size, m_index);
    m_index += size;
}

} // namespace ASL
