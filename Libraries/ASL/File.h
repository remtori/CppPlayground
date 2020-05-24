#pragma once

#include <ASL/ByteBuffer.h>
#include <ASL/String.h>
#include <ASL/Types.h>

namespace ASL {

class File {
public:
    enum OpenMode {
        Invalid = 0,
        Read = 1 << 0,
        Write = 1 << 1,
        Append = 1 << 3,
        Truncate = 1 << 4,
    };

    File(const String& filename);
    ~File();

    bool open(int open_mode = Read | Write);
    bool close();

    int mode() const { return m_open_mode; }
    bool is_open() const { return m_open_mode != Invalid; }
    bool is_closed() const { return m_closed; }

    String filename() const { return m_filename; }

    size_t read(u8* buffer, size_t size);
    ByteBuffer read(size_t max_size);
    ByteBuffer read_all();

    bool write(const u8* buffer, size_t size);
    bool write(const StringView&);

    bool can_read() const { return !m_eof; }

private:
    String m_filename;
    int m_open_mode = Invalid;
    bool m_closed = false;
    bool m_eof = false;
    void* m_fd = nullptr;
};

} // namespace ASL

using ASL::File;
