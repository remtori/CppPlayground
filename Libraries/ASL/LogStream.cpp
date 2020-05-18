#include "LogStream.h"

#include "FileSystemPath.h"
#include "SharedString.h"
#include "StdLibExtras.h"
#include "String.h"
#include <stdio.h>

namespace ASL {

static String process_name = "unknown";

void set_process_name(int argc, char** argv)
{
    ASSERT(argc > 0);
    process_name = FileSystemPath::basename(argv[0]);
}

void set_process_name(const char* name)
{
    process_name = String(name);
}

DebugStream dbg(const char* name)
{
    DebugStream stream;
    stream << "\033[33;1m[" << (name ? name : process_name.characters()) << "]\033[0m: ";
    return stream;
}

DebugStream::~DebugStream()
{
    putchar('\n');
}

DebugStream& DebugStream::operator<<(bool b)
{
    printf(b ? "true" : "false");
    return *this;
}

DebugStream& DebugStream::operator<<(char c)
{
    printf("%c", c);
    return *this;
}

DebugStream& DebugStream::operator<<(const char* cstring)
{
    printf("%s", cstring);
    return *this;
}

DebugStream& DebugStream::operator<<(int v)
{
    printf("%d", v);
    return *this;
}

DebugStream& DebugStream::operator<<(unsigned v)
{
    printf("%u", v);
    return *this;
}

DebugStream& DebugStream::operator<<(long v)
{
    printf("%ld", v);
    return *this;
}

DebugStream& DebugStream::operator<<(long unsigned int v)
{
    printf("%lu", v);
    return *this;
}

DebugStream& DebugStream::operator<<(long long v)
{
    printf("%lld", v);
    return *this;
}

DebugStream& DebugStream::operator<<(long long unsigned int v)
{
    printf("%llu", v);
    return *this;
}

DebugStream& DebugStream::operator<<(float v)
{
    printf("%.4f", v);
    return *this;
}

DebugStream& DebugStream::operator<<(double v)
{
    printf("%.4lf", v);
    return *this;
}

DebugStream& DebugStream::operator<<(const StringImpl& s)
{
    printf("%s", s.characters());
    return *this;
}

DebugStream& DebugStream::operator<<(const StringImpl* s)
{
    printf("%s", s->characters());
    return *this;
}

DebugStream& DebugStream::operator<<(const StringView& s)
{
    printf("%.*s", (int)s.length(), s.characters_wont());
    return *this;
}

DebugStream& DebugStream::operator<<(const SharedString& s)
{
    printf("%s", s.characters());
    return *this;
}

DebugStream& DebugStream::operator<<(const String& s)
{
    printf("%s", s.characters());
    return *this;
}

} // namespace ASL
