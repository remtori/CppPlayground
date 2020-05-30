#include <ASL/Serializable.h>

#include <ASL/String.h>

namespace ASL {

String Serializable::to_string() const
{
    ByteStream stream;
    serialize(stream);
    return String(stream.buffer());
}

} // namespace ASL
