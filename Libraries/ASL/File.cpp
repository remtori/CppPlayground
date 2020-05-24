#include <ASL/File.h>

#include <ASL/LogStream.h>
#include <stdio.h>

namespace ASL {

File::File(const String& filename)
    : m_filename(filename)
{
}

File::~File()
{
    if (!m_closed)
        close();
}

bool File::open(int open_mode)
{
    m_open_mode = open_mode;
    char mode[] = { 0, 'b', '+', 0 };
    if (open_mode & OpenMode::Read)
        mode[0] = 'r';
    if (open_mode & OpenMode::Write)
        mode[0] = 'w';
    if (open_mode & OpenMode::Append)
        mode[0] = 'a';
    if (open_mode & OpenMode::Truncate)
        mode[2] = 0;

    m_fd = fopen(m_filename.characters(), mode);
    if (m_fd)
        return true;

    m_open_mode = Invalid;
    return false;
}

bool File::close()
{
    int r = fclose((FILE*)m_fd);
    m_closed = true;
    return r == 0;
}

size_t File::read(u8* buffer, size_t size)
{
    size_t new_size = fread(buffer, 1, size, (FILE*)m_fd);
    if (new_size < size)
        m_eof = true;

    return new_size;
}

ByteBuffer File::read(size_t max_size)
{
    ByteBuffer buffer(max_size);
    int new_size = read(buffer.data(), max_size);
    buffer.trim(new_size);
    return buffer;
}

ByteBuffer File::read_all()
{
    constexpr const size_t chunk_size = 512;
    ByteBuffer buffer(chunk_size);

    size_t offset = 0;
    while (!feof((FILE*)m_fd)) {
        offset += read(&buffer[offset], chunk_size);
        buffer.grow(chunk_size);
    }

    buffer.trim(offset);
    return buffer;
}

bool File::write(const u8* buffer, size_t size)
{
    return fwrite(buffer, sizeof(u8), size, (FILE*)m_fd) == size;
}

bool File::write(const StringView& str)
{
    return write((u8*)str.characters_wont(), str.length());
}

} // namespace ASL
