#!/bin/bash

path=`pwd`

cd $path/build/
cmake ../cmake -DCPM_kiwaku_SOURCE=/home/higepi/stage/kiwaku -G Ninja -DCMAKE_CXX_COMPILER=clang++-15
ninja -v kmeans1D.out

for i in 10 20 50 100 200 500 1000
do
  ./kmeans1D.out -s $i -B
done