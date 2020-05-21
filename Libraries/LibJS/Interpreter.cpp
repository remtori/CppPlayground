#include "Interpreter.h"

namespace JS {

Value Interpreter::run(Expression& expression)
{
    return expression.run(*this);
}

void Interpreter::set_variable(const SharedString& identifier, Value value)
{
    m_variables.set(identifier, value);
}

Value Interpreter::get_variable(const SharedString& identifier)
{
    return m_variables.get_or(identifier, js_undefined());
}

} // namespace JS
