#pragma once
#include <ASL/Forward.h>
#include <ASL/HashMap.h>
#include <ASL/NonnullOwnPtr.h>
#include <ASL/SharedString.h>
#include <LibJS/AST.h>
#include <LibJS/Forward.h>
#include <LibJS/Heap/Heap.h>
#include <LibJS/Runtime/Object.h>
#include <LibJS/Runtime/Value.h>

namespace JS {

class Interpreter {
public:
    static NonnullOwnPtr<Interpreter> create()
    {
        auto interpreter = adopt_own(*new Interpreter);
        interpreter->m_global_object = interpreter->heap().allocate<Object>();
        return interpreter;
    }

    ~Interpreter() {}

    Value run(Statement&);
    void set_variable(const SharedString& identifier, Value value);
    Value get_variable(const SharedString& identifier);

    Heap& heap() { return m_heap; }

private:
    Interpreter();

    Heap m_heap;
    Object* m_global_object;
    HashMap<SharedString, Value> m_variables;
};

} // namespace JS
