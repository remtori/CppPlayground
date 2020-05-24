#include <ASL/JsonObject.h>

#include <ASL/StringBuilder.h>

namespace ASL {

void JsonObject::serialize(StringBuilder& builder, int space, int indent) const
{
    builder.append('{');
    if (space > 0)
        builder.append("\r\n");

    auto it = begin();
    for (;;) {
        builder.append_repeated(' ', space * (indent + 1));
        builder.append('"');
        builder.append(it->key);
        builder.append('\"');
        builder.append(':');

        if (space > 0)
            builder.append(' ');

        it->value.serialize(builder, space, indent + 1);

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
    builder.append('}');
}

String JsonObject::to_string(int space) const
{
    StringBuilder builder;
    serialize(builder, space);
    return builder.to_string();
}

} // namespace ASL
