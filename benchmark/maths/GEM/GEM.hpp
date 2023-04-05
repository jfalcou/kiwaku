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

// array of 2^MAX_SIZE
#define MAX_SIZE 9

// Default size of 32
extern int ARRAY_SIZE;
extern int SIZE_X;
extern int SIZE_Y;

// MHz
extern int Freq_CPU;

// Benchmarking
extern bool BENCHMARK;
extern bool UNITTEST;
extern unsigned int num_times;
extern size_t s_align;

// Data initialisation
template <class T>
void init_arrays(
    T *__restrict a,
    T *__restrict b)
{
  const int array_size = ARRAY_SIZE;
  for (int i = 0; i < array_size; i++)
  {
    a[i] = (T) 0;
    b[i] = (T)(rand()%10)/10;
  }
}

template <class T>
void init_matrix(
    T *__restrict a,
    T *__restrict b,
    T *__restrict c)
{
  const int array_size = ARRAY_SIZE;
  for (int j = 0; j < array_size; j++)
  {
    for (int i = 0; i < array_size; i++)
    {
      a[j*array_size+i] = (T) 0;
      b[j*array_size+i] = (T)(rand()%10)/10;
      c[j*array_size+i] = (T)(rand()%10)/10;
    }
  }
}

template <class T>
void print_init(std::vector<size_t> sizes, int n)
{
  std::streamsize ss = std::cout.precision();

  std::cout << "Running kernels " << num_times << " times" << std::endl;
  if (sizeof(T) == sizeof(float))
    std::cout << "Precision: float" << std::endl;
  else
    std::cout << "Precision: double" << std::endl;

  int tot_size = 0;
  for(int i=0;i<n;i++)tot_size+=sizes[i];

  // MB = 10^6
  std::cout << std::setprecision(1) << std::fixed
            << "Array size: " << ARRAY_SIZE * sizeof(T) * 1.0E-6 << " MB"
            << " (=" << ARRAY_SIZE * sizeof(T) * 1.0E-9 << " GB)" << std::endl;
  std::cout << std::setprecision(1) << std::fixed
            << "Matrix size: " << ARRAY_SIZE * ARRAY_SIZE * sizeof(T) * 1.0E-6 << " MB"
            << " (=" << ARRAY_SIZE * ARRAY_SIZE * sizeof(T) * 1.0E-9 << " GB)" << std::endl;
  std::cout << "Total size moved: " << tot_size * 1.0E-6 << " MB"
            << " (=" << tot_size * 1.0E-9 << " GB)" << std::endl;
  std::cout.precision(ss);
}

int parseUInt(const char *str, unsigned int *output);
int parseInt(const char *str, int *output);
void parseArguments(int argc, char *argv[]);