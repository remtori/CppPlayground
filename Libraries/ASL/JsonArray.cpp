#include <ASL/JsonArray.h>

#include <ASL/StringBuilder.h>

namespace ASL {

void JsonArray::serialize(StringBuilder& builder, int space, int indent) const
{
    builder.append('[');
    if (space > 0)
        builder.append("\r\n");

    auto it = begin();
    for (;;) {
        builder.append_repeated(' ', space * (indent + 1));
        it->serialize(builder, space, indent + 1);

        ++it;
        if (it != end()) {
            builder.append(',');
            if (space > 0)
                builder.append("\r\n");
        } else {
            break;
        }
    }

    if (space > 0)
        builder.append("\r\n");

    builder.append_repeated(' ', space * indent);
    builder.append(']');
}

String JsonArray::to_string(int space) const
{
    StringBuilder builder;
    serialize(builder, space);
    return builder.to_string();
}

} // namespace ASL