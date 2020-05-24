#pragma once

#include <ASL/String.h>

namespace ASL {

class LIB_API FileSystemPath {
public:
    static FileSystemPath parse(const StringView&);
    static String dirname(const StringView&);
    static String basename(const StringView&);
    static String filename(const StringView&);
    static String extension(const StringView&);

    FileSystemPath() = delete;
    explicit FileSystemPath(const StringView&);

    bool is_absolute() const { return m_is_absolute; }
    const String& string() const { return m_string; }

    const String& root() const { return m_root; }
    const String& dirname() const { return m_dirname; }
    const String& basename() const { return m_basename; }
    const String& filename() const { return m_filename; }
    const String& extension() const { return m_extension; }

    bool has_extension(const StringView&) const;

private:
    void resolve();

    String m_string;
    String m_root;
    String m_dirname;
    String m_basename;
    String m_filename;
    String m_extension;
    bool m_is_absolute = false;
};

}; // namespace ASL

using ASL::FileSystemPath;
