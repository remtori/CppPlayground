#pragma once

#include "AST.h"
#include "Forward.h"
#include <ASL/Forward.h>
#include <ASL/HashMap.h>
#include <ASL/NonnullOwnPtr.h>
#include <ASL/SharedString.h>

namespace JS {

class Interpreter {
public:
    static NonnullOwnPtr<Interpreter> create()
    {
        return adopt_own(*new Interpreter);
    }

    ~Interpreter() {}

    Value run(Expression&);
    void set_variable(const SharedString& identifier, Value value);
    Value get_variable(const SharedString& identifier);

private:
    Interpreter() {}
    HashMap<SharedString, Value> m_variables;
};

} // namespace JS
