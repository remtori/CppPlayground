#pragma once

#include <ASL/ByteStream.h>
#include <ASL/LogStream.h>

namespace ASL {

class Serializable {
public:
    virtual void serialize(ByteStream&) const = 0;
    virtual void deserialize(ByteStream&) = 0;
    virtual String to_string() const;
};

const DebugStream& operator<<(const DebugStream& stream, const Serializable& s)
{
    return stream << s.to_string();
};

const DebugStream& operator<<(const DebugStream& stream, const Serializable* s)
{
    return stream << s->to_string();
};

} // namespace ASL

using ASL::Serializable;
