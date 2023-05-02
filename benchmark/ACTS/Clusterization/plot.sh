#!/bin/bash

path=`pwd`

for i in 2 4 10 20 50 100 1000
do
  python3 plot_compare.py $path/standalone/results/*$i.csv $path/kwk/results/*$i.csv
done