#include <LibJS/Runtime/Object.h>

#include <ASL/StringBuilder.h>

namespace JS {

Object::Object()
{
}

Object::~Object()
{
    clear();
}

Value Object::get_own_property(const SharedString& property_name)
{
    return m_properties.get_or(property_name, js_undefined());
}

void Object::put_own_property(const SharedString& property_name, const Value& value)
{
    m_properties.set(property_name, value);
}

void Object::visit()
{
    Cell::visit();
    for (auto& kv : m_properties) {
        kv.value.visit();
    }
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
