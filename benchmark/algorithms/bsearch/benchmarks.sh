#!/bin/bash

path=`pwd`

cd $path/standalone/build/

cmake ../cmake/
make 
./*.out -s 0 -n 10 -f 1600

cd $path/kwk_table/build/

cmake ../cmake -DCPM_kiwaku_SOURCE=/home/higepi/stage/kiwaku -G Ninja -DCMAKE_CXX_COMPILER=clang++-15
`ninja -v kwk.out`
./*.out -s 0 -n 10 -f 1600

cd $path/kwk_view/build/

cmake ../cmake -DCPM_kiwaku_SOURCE=/home/higepi/stage/kiwaku -G Ninja -DCMAKE_CXX_COMPILER=clang++-15
`ninja -v kwk.out`
./*.out -s 0 -n 10 -f 1600

cd $path/compare/

python3 ../plot_compare.py ../standalone/results/Benchmark_std_nano_float.csv ../kwk_table/results/Benchmark_std_nano_float.csv ../kwk_view/results/Benchmark_std_nano_float.csv 
python3 ../plot_compare.py ../standalone/results/Benchmark_std_nano_double.csv ../kwk_table/results/Benchmark_std_nano_double.csv ../kwk_view/results/Benchmark_std_nano_double.csv 