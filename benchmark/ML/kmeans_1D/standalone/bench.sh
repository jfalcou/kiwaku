#!/bin/bash

path=`pwd`

cd $path/build/
cmake ../cmake
make

./kmeans1D.out -s 10 -B