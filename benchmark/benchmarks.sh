#!/bin/bash

path=`pwd`

# Babelstream
cd $path/bandwidth/babelstream/standalone/build
make

cd $path/bandwidth/babelstream/standalone/results
../build/std.out -s 0 -n 100 -f 1600
python3 ../../plot.py B*.csv

cd $path/bandwidth/babelstream/kwk_view/build
ninja -v kwk.out

cd $path/bandwidth/babelstream/kwk_view/results
../build/kwk.out -s 0 -n 100 -f 1600
python3 ../../plot.py B*.csv

cd $path/bandwidth/babelstream/kwk_table/build
ninja -v kwk.out

cd $path/bandwidth/babelstream/kwk_table/results
../build/kwk.out -s 0 -n 100 -f 1600
python3 ../../plot.py B*.csv

# GEM
cd $path/maths/GEM/standalone/build
make

cd $path/maths/GEM/standalone/results
../build/std.out -s 0 -n 100 -f 1600
python3 ../../plot.py B*.csv

cd $path/maths/GEM/kwk_view/build
ninja -v kwk.out

cd $path/maths/GEM/kwk_view/results
../build/kwk.out -s 0 -n 100 -f 1600
python3 ../../plot.py B*.csv

cd $path/maths/GEM/kwk_table/build
ninja -v kwk.out

cd $path/maths/GEM/kwk_table/results
../build/kwk.out -s 0 -n 100 -f 1600
python3 ../../plot.py B*.csv
