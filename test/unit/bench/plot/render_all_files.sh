#!/bin/bash

FILE_EXTENSION="bench"

# should be:   for f in *.MTS;
for f in files/*.$FILE_EXTENSION;
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
  python3 plot_bench.py $f -s

done