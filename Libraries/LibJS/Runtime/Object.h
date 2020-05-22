#pragma once

#include "Value.h"
#include <ASL/HashMap.h>
#include <ASL/RefCounted.h>
#include <ASL/SharedString.h>
#include <ASL/String.h>

namespace JS {

class Object : public RefCounted<Object> {
public:
    Object();
    ~Object();

    Value get_own_property(const SharedString& property_name);
    void put_own_property(const SharedString& property_name, const Value& value);

    virtual Value call(Interpreter& interpreter) { return js_undefined(); };
    virtual bool is_function() const { return false; }

    String to_string() const;

private:
    void clear();

    HashMap<SharedString, Value> m_properties;
};

} // namespace JS
