#pragma once

#include <ASL/HashMap.h>
#include <ASL/SharedString.h>
#include <ASL/String.h>
#include <LibJS/Heap/Cell.h>
#include <LibJS/Runtime/Value.h>

namespace JS {

class Object : public Cell {
public:
    Object();
    ~Object();

    Value get_own_property(const SharedString& property_name);
    void put_own_property(const SharedString& property_name, const Value& value);

    virtual Value call(Interpreter& interpreter) { return js_undefined(); };
    virtual bool is_function() const { return false; }

    virtual void visit() override;

    String to_string() const;

private:
    void clear();

    HashMap<SharedString, Value> m_properties;
};

} // namespace JS
