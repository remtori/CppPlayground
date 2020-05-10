#pragma once

#include "CallToString.h"
#include "Forward.h"
#include "String.h"

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
        String str = "<unknown-type>";
        call_to_string_if_present(&any, str);
        return *this << str;
    }
};

DebugStream& dbg(const char* name = nullptr);
void set_process_name(int argc, char** argv);
void set_process_name(const char* name);

} // namespace ASL

using ASL::dbg;
