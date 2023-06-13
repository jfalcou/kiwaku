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