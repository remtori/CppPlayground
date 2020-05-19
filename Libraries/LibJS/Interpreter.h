#pragma once

#include "AST.h"
#include "Forward.h"
#include <ASL/NonnullOwnPtr.h>

namespace JS {

class Interpreter {
public:
    static NonnullOwnPtr<Interpreter> create()
    {
        return adopt_own(*new Interpreter);
    }

    ~Interpreter() {}

    JSValue run(Expression&);

private:
    Interpreter() {}
};

} // namespace JS
