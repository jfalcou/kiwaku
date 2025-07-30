#!/bin/bash

FILE_EXTENSION="bench"

# PATH="files/2025-06-19-legend/renamed2/"

# PATH="final_files/transform/"
# PATH="final_files/transform_trigo_v2/"
# PATH="final_files/reduce/"
# PATH="final_files/find_if/"
PATH="final_files/transform_v3/"
PATH="final_files/transform_reduce/fast/"
# PATH="files/avx512/render/"

# should be:   for f in *.MTS;
for f in ${PATH}*.$FILE_EXTENSION;
do

  # No matching file
  if [ "$f" == "*.$FILE_EXTENSION" ]; 
  then
    echo ">>>>>>>>>>> No \".$FILE_EXTENSION\" file.";
    echo ""
    break;
  fi

  echo ""
  echo "Rendering ${f}:"
  /usr/bin/python3 plot_bench2.py $f -s

done

# python3 plot_bench.py files/2025-06-19-legend/renamed2/
# parsys-legend_2025-06-19_22h23m13s_transform_reduce_compute-bound.bench
# parsys-legend_2025-06-19_22h26m32s_transform_reduce_memory-bound.bench