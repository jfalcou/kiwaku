#pragma once
#include <ios>
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <cstdint>
#include <math.h>
#include <unistd.h>

// scalar constanst for the mul, triad and nstream kernels
#define SCALAR (0.4)

// Bench array size 
#define MAX_SIZE 9

extern int ARRAY_SIZE;
extern int Freq_CPU;
// Benchmarking
extern bool BENCHMARK;
extern unsigned int num_times;


template <class T>
void init_arrays(
    T *__restrict a,
    T *__restrict b,
    T *__restrict c,
    T initA, T initB, T initC)
{
  const int array_size = ARRAY_SIZE;
  for (int i = 0; i < array_size; i++)
  {
    a[i] = initA;
    b[i] = initB;
    c[i] = initC;
  }
}

int parseUInt(const char *str, unsigned int *output);
int parseInt(const char *str, int *output);
void parseArguments(int argc, char *argv[]);