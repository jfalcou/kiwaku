# Kmeans

Clustering algorithm

## Compile

### STD

```bash
cd $/build/
cmake ../cmake
make
```

### KWK

```bash
cd $/build/
cmake ../cmake -DCPM_kiwaku_SOURCE=/home/higepi/stage/kiwaku -G Ninja -DCMAKE_CXX_COMPILER=clang++-15
ninja -v kwk.out
```

## Arguments

| Arg | Description | Default |  
| --- | ----------- | ------- | 
| -s --arraysize | Number of elements > 0 (2^s)| 100 | 
| -f --freq | Frequency (Mhz) > 0| 1600 |
| -n --nuntimes | Number of nanobench iteration >= 1| 2 |
| -S --seed | Random generation seed > 0| 1618 | 
| -B --benchmark | Benchmark mode (launch with -B) | false | 
| -h --help | Help |  | 


## scripts

### subscripts

1. bench.sh : launch .out for benchmark
2. test.sh  : launch .out for unit test

## Benchmark

Sums the cycles count for 1 to K clusters, K clusters being N points.
N points goes from 2 to 2^arraysize.