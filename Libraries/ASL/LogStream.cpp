#include <ASL/LogStream.h>

#include <ASL/FileSystemPath.h>
#include <ASL/SharedString.h>
#include <ASL/StdLibExtras.h>
#include <ASL/String.h>
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

const DebugStream& operator<<(const DebugStream& stream, bool b)
{
    printf(b ? "true" : "false");
    return stream;
}

const DebugStream& operator<<(const DebugStream& stream, char c)
{
    printf("%c", c);
    return stream;
}

const DebugStream& operator<<(const DebugStream& stream, const char* cstring)
{
    printf("%s", cstring);
    return stream;
}

const DebugStream& operator<<(const DebugStream& stream, int v)
{
    printf("%d", v);
    return stream;
}

const DebugStream& operator<<(const DebugStream& stream, unsigned v)
{
    printf("%u", v);
    return stream;
}

const DebugStream& operator<<(const DebugStream& stream, long v)
{
    printf("%ld", v);
    return stream;
}

const DebugStream& operator<<(const DebugStream& stream, long unsigned int v)
{
    printf("%lu", v);
    return stream;
}

const DebugStream& operator<<(const DebugStream& stream, long long v)
{
    printf("%lld", v);
    return stream;
}

const DebugStream& operator<<(const DebugStream& stream, long long unsigned int v)
{
    printf("%llu", v);
    return stream;
}

const DebugStream& operator<<(const DebugStream& stream, float v)
{
    printf("%.4f", v);
    return stream;
}

const DebugStream& operator<<(const DebugStream& stream, double v)
{
    printf("%.4lf", v);
    return stream;
}

const DebugStream& operator<<(const DebugStream& stream, const StringImpl& s)
{
    printf("%s", s.characters());
    return stream;
}

const DebugStream& operator<<(const DebugStream& stream, const StringImpl* s)
{
    printf("%s", s->characters());
    return stream;
}

const DebugStream& operator<<(const DebugStream& stream, const StringView& s)
{
    printf("%.*s", (int)s.length(), s.characters_wont());
    return stream;
}

const DebugStream& operator<<(const DebugStream& stream, const SharedString& s)
{
    printf("%s", s.characters());
    return stream;
}

const DebugStream& operator<<(const DebugStream& stream, const String& s)
{
    printf("%s", s.characters());
    return stream;
}

} // namespace ASL
