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
#define MAX_SIZE 16

extern int ARRAY_SIZE;
extern int Freq_CPU;
// Benchmarking
extern bool BENCHMARK;
extern unsigned int num_times;


template <class T>
void init_arrays(T *__restrict a)
{
  const int array_size = ARRAY_SIZE;
  for (int i = 0; i < array_size; i++)
  {
    a[i] = i;
  }
}

int parseUInt(const char *str, unsigned int *output);
int parseInt(const char *str, int *output);
void parseArguments(int argc, char *argv[]);