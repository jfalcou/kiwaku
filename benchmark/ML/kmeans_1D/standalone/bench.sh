#!/bin/bash

path=`pwd`

cd $path/build/
cmake ../cmake
make

for i in 10 20 50 100 200 500 1000
do
  ./kmeans1D.out -s $i -B
done