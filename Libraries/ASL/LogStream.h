#pragma once

#include "Forward.h"
#include "String.h"
#include "TryToString.h"

namespace ASL {

class DebugStream {
public:
    DebugStream() {}
    ~DebugStream() {}

    DebugStream& operator<<(char);
    DebugStream& operator<<(const char*);
    DebugStream& operator<<(int);
    DebugStream& operator<<(unsigned);
    DebugStream& operator<<(long);
    DebugStream& operator<<(long unsigned int);
    DebugStream& operator<<(long long);
    DebugStream& operator<<(long long unsigned int);
    DebugStream& operator<<(float);
    DebugStream& operator<<(double);
    DebugStream& operator<<(const StringImpl&);
    DebugStream& operator<<(const StringImpl*);
    DebugStream& operator<<(const StringView&);
    DebugStream& operator<<(const SharedString&);
    DebugStream& operator<<(const String&);

    template<typename T>
    DebugStream& operator<<(const T& any)
    {
        return *this << try_to_string(any);
    }
};

DebugStream& dbg(const char* name = nullptr);
void set_process_name(int argc, char** argv);
void set_process_name(const char* name);

} // namespace ASL

using ASL::dbg;
