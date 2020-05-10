#include "FileSystemPath.h"

#include "StringBuilder.h"
#include "Vector.h"

namespace ASL {

FileSystemPath FileSystemPath::parse(const StringView& s)
{
    return FileSystemPath(s);
}

String FileSystemPath::dirname(const StringView& s)
{
    FileSystemPath p(s);
    return p.dirname();
}

String FileSystemPath::basename(const StringView& s)
{
    FileSystemPath p(s);
    return p.basename();
}

String FileSystemPath::filename(const StringView& s)
{
    FileSystemPath p(s);
    return p.filename();
}

String FileSystemPath::extension(const StringView& s)
{
    FileSystemPath p(s);
    return p.extension();
}

FileSystemPath::FileSystemPath(const StringView& s)
    : m_string(s)
{
    resolve();
}

void FileSystemPath::resolve()
{
    if (m_string.is_empty())
        return;

    if (m_string[0] == '/') {
        m_is_absolute = true;
        m_root = "/";
    }

    auto parts = m_string.split_view('/');
    if (parts.size() > 1) {
        StringBuilder dir_builder;

        if (m_is_absolute)
            dir_builder.append('/');

        for (size_t i = 0; i < parts.size() - 1; ++i) {
            dir_builder.append(parts[i]);
            if (i < parts.size() - 2)
                dir_builder.append('/');
        }

        m_dirname = dir_builder.to_string();
    }

    m_basename = parts[parts.size() - 1];
    auto pos = m_basename.find_last_of('.');

    if (pos.has_value()) {
        size_t i = pos.value();
        m_filename = m_basename.substring(0, i);
        m_extension = m_basename.substring(i);
    } else {
        m_filename = m_basename;
    }
}

bool FileSystemPath::has_extension(const StringView& extension) const
{
    return m_string.ends_with(extension, false);
}

String resolved_path(const StringView& path)
{
    return FileSystemPath(path).string();
}

} // namespace ASL
