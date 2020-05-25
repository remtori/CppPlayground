#pragma once

#include <LibJS/Runtime/Value.h>

namespace JS {

// Binary operator
Value b_op_strict_equals(const Value& a, const Value& b);
Value b_op_equals(const Value& a, const Value& b);
Value b_op_add(const Value& a, const Value& b);
Value b_op_sub(const Value& a, const Value& b);
Value b_op_mult(const Value& a, const Value& b);
Value b_op_div(const Value& a, const Value& b);
Value b_op_mod(const Value& a, const Value& b);
Value b_op_pow(const Value& a, const Value& b);
Value b_op_left_shift(const Value& a, const Value& b);
Value b_op_right_shift(const Value& a, const Value& b);
Value b_op_unsigned_right_shift(const Value& a, const Value& b);
Value b_op_less_than(const Value& a, const Value& b);
Value b_op_greater_than(const Value& a, const Value& b);
Value b_op_less_than_or_equals(const Value& a, const Value& b);
Value b_op_greater_than_or_equals(const Value& a, const Value& b);
Value b_op_bit_and(const Value& a, const Value& b);
Value b_op_bit_or(const Value& a, const Value& b);
Value b_op_bit_xor(const Value& a, const Value& b);

// Unary operator
Value u_op_void(const Value& v);
Value u_op_typeof(const Value& v);
Value u_op_plus(const Value& v);
Value u_op_minus(const Value& v);
Value u_op_bit_not(const Value& v);
Value u_op_not(const Value& v);

} // namespace JS
