#!/bin/bash

path=`pwd`

cd $path/build/
cmake ../cmake -DCPM_kiwaku_SOURCE=/home/higepi/stage/kiwaku -G Ninja -DCMAKE_CXX_COMPILER=clang++-15
ninja -v kmeans1D.out

./kmeans1D.out -s 10