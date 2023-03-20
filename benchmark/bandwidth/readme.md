# Bandwidth benchmark

## babelstream

Based on https://github.com/zjin-lcf/HeCBench/tree/master/babelstream-sycl

Compute bandwidth on functions :
    - Copy, Y = X
    - Multiply, Y = A*X
    - Add, Y += X
    - Triad, Y = A.X + B
    - nstream, Y += A*X + B
    - Dot multiply, Y = A.X