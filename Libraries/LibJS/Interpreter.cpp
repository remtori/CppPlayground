#include "Interpreter.h"

namespace JS {

JSValue Interpreter::run(Expression& expression)
{
    return expression.run(*this);
}

} // namespace JS
