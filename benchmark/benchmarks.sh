#!/bin/bash

path=`pwd`

# Babelstream
cd $path/bandwidth/babelstream/standalone/results

../build/std.out -s 0 -n 100 -f 1600

cd $path/bandwidth/babelstream/kwk_view/results

../build/kwk.out -s 0 -n 100 -f 1600

cd $path/bandwidth/babelstream/kwk_table/results

../build/kwk.out -s 0 -n 100 -f 1600

# GEM
cd $path/maths/GEM/standalone/results

../build/std.out -s 0 -n 100 -f 1600

cd $path/maths/GEM/kwk_view/results

../build/kwk.out -s 0 -n 100 -f 1600

cd $path/maths/GEM/kwk_table/results

../build/kwk.out -s 0 -n 100 -f 1600
