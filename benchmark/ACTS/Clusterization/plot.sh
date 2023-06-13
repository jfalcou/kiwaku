#!/bin/bash

path=`pwd`

for i in 10 20 50 100 200 500 1000
do
  python3 plot_compare.py $path/standalone/results/Bench*$i.csv $path/kwk_def/results/Bench*$i.csv
done