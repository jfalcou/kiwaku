#!/bin/sh -l

set -e
echo "::group::Running: 'cmake .. -G Ninja -DCMAKE_CXX_COMPILER="$1" $3'"
mkdir build
cd build
cmake .. -G Ninja -DCMAKE_CXX_COMPILER=$1 $3
echo "::endgroup::"

echo "::group::Compiling tests"
ninja -j $2
echo "::endgroup::"

echo "::group::Running tests"
ctest -j $2
echo "::endgroup::"

exit 0
