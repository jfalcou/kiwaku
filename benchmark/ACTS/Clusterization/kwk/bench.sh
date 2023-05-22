#!/bin/bash

path=`pwd`

cd $path/build/
cmake ../cmake -DCPM_kiwaku_SOURCE=/home/higepi/stage/kiwaku -G Ninja -DCMAKE_CXX_COMPILER=clang++-15
ninja -v Cluster.out

cd $path/results

# for i in 2 4 10 20 50 100 1000
# do
#   ../build/Cluster.out $i
# done

##debug

../build/Cluster.out 4