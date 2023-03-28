# Bandwidth benchmark

https://en.algorithmica.org/hpc/cpu-cache/bandwidth/

## babelstream

Based on https://github.com/zjin-lcf/HeCBench/tree/master/babelstream-sycl

Compute bandwidth on functions :
    - Copy, Y = X
    - Multiply, Y = A*X
    - Add, Y += X
    - Triad, Y = A.X + B
    - nstream, Y += A*X + B
    - Dot multiply, Y = A.X

## Intel i7-10710U
https://www.intel.com/content/www/us/en/products/sku/196448/intel-core-i710710u-processor-12m-cache-up-to-4-70-ghz/specifications.html

Cores : 6
Threads : 12
Frequency max (1 core): 4.7 GHz
Frequency opt : 4.1 GHz
Caches : L1 64 KB, L2 256 KB, L3 12 MB
Max Memory Bandwidth : 45.8 GB/s (RAM)

### CPU-X 
L1 : 102 GB/s
L2 : 68 GB/s
L3 : 45 GB/s

## Compilation

### standalone 

``` 
g++-11 -O3 babelstream_standalone.cpp -o standalone.o
```

### kiwaku

Compile with kiwaku :

```
cmake ../cmake -DCPM_kiwaku_SOURCE=/home/higepi/stage/kiwaku -G Ninja -DCMAKE_CXX_COMPILER=clang++-15
```

Make launch
``` 
`ninja -v kwk.out`
```