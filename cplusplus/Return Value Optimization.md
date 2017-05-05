#Return Value Optimization
- If local variable is return, the local variable is actually the same variable at the callsite holding the result
- Return the local variable using rlvalue, move semantics.
- Return the local variable using lvalue, copy semantics.
