# Binary search

Searching for element x in a N-element array of type T. The worst case scenario is O(log n).

## Benchmark

The benchmark execute the search

## build

STD build/
```cmake
cmake ../cmake/
make 
```

KWK In the project build/
```
cmake ../cmake -DCPM_kiwaku_SOURCE=/home/higepi/stage/kiwaku -G Ninja -DCMAKE_CXX_COMPILER=clang++-15
```

Make launch
``` 
`ninja -v kwk.out`
```

## Launch
-s (size) : 0 for benchmode, >= 2 normal mode. Normal mode has a verifying test on the result
-n (iterations/epochs) : 100 is a good number 
-f (cpu frequency) : MHz
`./*.o -s 0 -n 100 -f 1600`



