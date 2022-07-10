#!/bin/sh -l

echo "::group::Running: 'cmake .. -G Ninja -DCMAKE_CXX_COMPILER="$1" $3'"
mkdir build
cd build
cmake .. -G Ninja -DCMAKE_CXX_COMPILER=$1 && ninja -j $2
if [ "$?" -ne "0" ]
then
  echo "::error Tests can not be compiled!" ;
  exit 1;
fi

ctest -j $2 $3
if [ "$?" -ne "0" ]
then
  echo "::error Tests failed!" ;
  exit 1;
fi

echo "::endgroup::"

exit 0
