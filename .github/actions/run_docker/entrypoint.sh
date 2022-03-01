#!/bin/sh -l

echo "::group::Running: 'cmake .. -G Ninja -DCMAKE_CXX_COMPILER="$1" $3'"
mkdir build
cd build
cmake .. -G Ninja -DCMAKE_CXX_COMPILER=$1 && ninja unit -j $2 && ctest -j $2 $3
echo "::endgroup::"

exit 0
