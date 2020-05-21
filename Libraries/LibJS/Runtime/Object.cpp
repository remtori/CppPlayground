#include "Object.h"

#include <ASL/StringBuilder.h>

namespace JS {

Object::Object()
{
}

Object::~Object()
{
    clear();
}

Value Object::get_property(const SharedString& property_name)
{
    return m_properties.get_or(property_name, js_undefined());
}

void Object::set_property(const SharedString& property_name, const Value& value)
{
    m_properties.set(property_name, value);
}

String Object::to_string() const
{
    // TODO: Implement
    return "[Object object]";
}

void Object::clear()
{
}

} // namespace JS
