#!/bin/sh -l

echo "::group::Running: 'cmake .. -G Ninja -DCMAKE_CXX_FLAGS="$1" $2'"
mkdir build
cd build
cmake .. -G Ninja -DCMAKE_CXX_COMPILER="$1"
echo "::endgroup::"

echo "::group::Compiling test" ;
ninja unit ;
echo "::endgroup::" ;

echo "::group::Running test" ;
ctest ;
echo "::endgroup::" ;
