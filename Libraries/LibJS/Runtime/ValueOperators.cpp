#include <LibJS/Runtime/Object.h>
#include <LibJS/Runtime/ValueOperators.h>

#include <ASL/LogStream.h>

namespace JS {

Value b_op_strict_equals(const Value& a, const Value& b)
{
    return Value(a.equals(b));
}

Value b_op_equals(const Value& a, const Value& b)
{
    if (a.type() == b.type())
        return b_op_strict_equals(a, b);

    if ((a.is_null() && b.is_undefined()) || (a.is_undefined() && b.is_null()))
        return Value(true);

    if (a.is_string() && b.is_number())
        return b_op_strict_equals(a.to_number(), b);

    if (a.is_number() && b.is_string())
        return b_op_strict_equals(a, b.to_number());

    if (a.is_bool())
        return b_op_equals(a.to_number(), b);

    if (b.is_bool())
        return b_op_equals(a, b.to_number());

    if ((a.is_string() || a.is_number()) && b.is_object())
        return b_op_equals(a, b.to_primitive());

    if (a.is_object() && (b.is_string() || b.is_number()))
        return b_op_equals(a.to_primitive(), b);

    return Value(false);
}

Value b_op_add(const Value& a, const Value& b)
{
    Value left = a;
    Value right = b;
    if (a.is_object())
        left = a.to_primitive();
    if (b.is_object())
        right = b.to_primitive();

    if (a.is_string() || b.is_string()) {
        // TODO: Implement string concatenation
        ASSERT_NOT_REACHED();
    }

    return Value(left.to_number().as_double() + right.to_number().as_double());
}

Value b_op_sub(const Value& a, const Value& b)
{
    return Value(a.to_number().as_double() - b.to_number().as_double());
}

Value b_op_mult(const Value& a, const Value& b)
{
    return Value(a.to_number().as_double() * b.to_number().as_double());
}

Value b_op_div(const Value& a, const Value& b)
{
    return Value(a.to_number().as_double() / b.to_number().as_double());
}

Value b_op_mod(const Value& a, const Value& b)
{
    double left = a.to_number().as_double();
    double right = b.to_number().as_double();
    return Value(left - (floor(left / right) * right));
}

Value b_op_pow(const Value& a, const Value& b)
{
    return Value(pow(a.to_number().as_double(), b.to_number().as_double()));
}

Value b_op_left_shift(const Value& a, const Value& b)
{
    return Value(a.to_number().to_i32() << b.to_number().to_u32());
}

Value b_op_right_shift(const Value& a, const Value& b)
{
    return Value(a.to_number().to_i32() >> b.to_number().to_u32());
}

Value b_op_unsigned_right_shift(const Value& a, const Value& b)
{
    return Value(a.to_number().to_u32() >> b.to_number().to_u32());
}

Value b_op_less_than(const Value& a, const Value& b)
{
    auto left = a.to_primitive(Value::PreferredType::Number);
    auto right = b.to_primitive(Value::PreferredType::Number);

    if (left.is_string() && right.is_string())
        return Value(strcmp(left.as_string().characters(), right.as_string().characters()) < 0);

    auto num_a = left.to_number();
    auto num_b = right.to_number();
    if (num_a.is_nan() || num_b.is_nan())
        return js_undefined();

    return Value(num_a.as_double() < num_b.as_double());
}

Value b_op_greater_than(const Value& a, const Value& b)
{
    auto left = a.to_primitive(Value::PreferredType::Number);
    auto right = b.to_primitive(Value::PreferredType::Number);

    if (left.is_string() && right.is_string())
        return Value(strcmp(left.as_string().characters(), right.as_string().characters()) > 0);

    auto num_a = left.to_number();
    auto num_b = right.to_number();
    if (num_a.is_nan() || num_b.is_nan())
        return js_undefined();

    return Value(num_a.as_double() > num_b.as_double());
}

Value b_op_less_than_or_equals(const Value& a, const Value& b)
{
    auto r = b_op_greater_than(a, b);
    if (r.is_undefined())
        return r;

    return Value(!r.as_bool());
}

Value b_op_greater_than_or_equals(const Value& a, const Value& b)
{
    auto r = b_op_less_than(a, b);
    if (r.is_undefined())
        return r;

    return Value(!r.as_bool());
}

Value b_op_bit_and(const Value& a, const Value& b)
{
    return Value(a.to_i32() & b.to_i32());
}

Value b_op_bit_or(const Value& a, const Value& b)
{
    return Value(a.to_i32() | b.to_i32());
}

Value b_op_bit_xor(const Value& a, const Value& b)
{
    return Value(a.to_i32() ^ b.to_i32());
}

Value u_op_void(const Value& v)
{
    return js_undefined();
}

Value u_op_typeof(const Value& v)
{
    if (v.is_undefined())
        return Value("undefined");

    if (v.is_null() || (v.is_object() && !v.as_object().is_function()))
        return Value("object");

    if (v.is_number())
        return Value("number");

    if (v.is_string())
        return Value("string");

    if (v.is_object() && v.as_object().is_function())
        return Value("function");

    dbg() << "Unknown Value passed to typeof";
    ASSERT_NOT_REACHED();
}

Value u_op_plus(const Value& v)
{
    return v.to_number();
}

Value u_op_minus(const Value& v)
{
    auto r = v.to_number();
    if (r.is_nan())
        return r;

    return Value(-r.as_double());
}

Value u_op_bit_not(const Value& v)
{
    return Value(~v.to_i32());
}

Value u_op_not(const Value& v)
{
    return Value(!v.to_bool());
}

} // namespace JS
