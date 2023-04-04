#!/bin/bash

clang++-15 -std=c++17 -O3 -L/usr/lib/x86_64-linux-gnu/blas -lblas ../GEM_std.cpp ../../GEM.cpp -o std.out