#pragma once

#include <ASL/Forward.h>
#include <ASL/String.h>

namespace ASL {

class LIB_API DebugStream {
public:
    DebugStream() {}
    ~DebugStream();
};

const DebugStream& operator<<(const DebugStream& stream, bool);
const DebugStream& operator<<(const DebugStream& stream, char);
const DebugStream& operator<<(const DebugStream& stream, const char*);
const DebugStream& operator<<(const DebugStream& stream, int);
const DebugStream& operator<<(const DebugStream& stream, unsigned);
const DebugStream& operator<<(const DebugStream& stream, long);
const DebugStream& operator<<(const DebugStream& stream, long unsigned int);
const DebugStream& operator<<(const DebugStream& stream, long long);
const DebugStream& operator<<(const DebugStream& stream, long long unsigned int);
const DebugStream& operator<<(const DebugStream& stream, float);
const DebugStream& operator<<(const DebugStream& stream, double);
const DebugStream& operator<<(const DebugStream& stream, const StringImpl&);
const DebugStream& operator<<(const DebugStream& stream, const StringImpl*);
const DebugStream& operator<<(const DebugStream& stream, const StringView&);
const DebugStream& operator<<(const DebugStream& stream, const SharedString&);
const DebugStream& operator<<(const DebugStream& stream, const String&);

LIB_API DebugStream dbg(const char* name = nullptr);
LIB_API void set_process_name(int argc, char** argv);
LIB_API void set_process_name(const char* name);

} // namespace ASL

using ASL::dbg;
