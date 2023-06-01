#pragma once
#include "../bsearch.hpp"
#include "kwk/container/table.hpp"
#include "kwk/kwk.hpp"
#include "kwk/utility/container/shape.hpp"
#include <kwk/concepts/container.hpp>

// Bench
#define ANKERL_NANOBENCH_IMPLEMENT
#include "../../../nanobench.h"

// Default size of 2^25
int ARRAY_SIZE = 100;
// MHz
int Freq_CPU = 1600;
// Benchmarking
bool BENCHMARK = false;
unsigned int num_times = 10;

std::ofstream res_nano;
std::ofstream res_chrono;

// Binary Search in C++
template <class T>
int binarySearch(T array[], T x, int low, int high) {
  
	// Repeat until the pointers low and high meet each other
  while (low <= high) {
    int mid = low + (high - low) / 2;

    if (array[mid] == x)
      return mid;

    if (array[mid] < x)
      low = mid + 1;

    else
      high = mid - 1;
  }

  return -1;
}

template <class T>
int binarySearchRecursive(T array[], T x, int low, int high) {
  if (high >= low) {
    int mid = low + (high - low) / 2;

    // If found at mid, then return it
    if (array[mid] == x)
      return mid;

    // Search the left half
    if (array[mid] > x)
      return binarySearchRecursive(array, x, low, mid - 1);

    // Search the right half
    return binarySearchRecursive(array, x, mid + 1, high);
  }

  return -1;
}

template <class T>
int verify(T x, int mid){
  if(x != mid) exit(EXIT_FAILURE);
  return -1;
}

// Runs the kernel(s) and prints output.
template <typename T>
void run()
{
  std::streamsize ss = std::cout.precision();

  std::cout << "Running kernels " << num_times << " times" << std::endl;

  const int array_size = ARRAY_SIZE;
  T *a = (T *)aligned_alloc(1024, sizeof(T) * array_size);
  auto kwkA = kwk::view{kwk::source = a, kwk::of_size(ARRAY_SIZE)};

  if (sizeof(T) == sizeof(int))
    std::cout << "Precision: int" << std::endl;
  else if (sizeof(T) == sizeof(float))
    std::cout << "Precision: float" << std::endl;
  else if (sizeof(T) == sizeof(double))
    std::cout << "Precision: double" << std::endl;

  // MB = 10^6
  std::cout << std::setprecision(1) << std::fixed
            << "Array size: " << ARRAY_SIZE * sizeof(T) * 1.0E-6 << " MB"
            << " (=" << ARRAY_SIZE * sizeof(T) * 1.0E-9 << " GB)" << std::endl;

  std::cout.precision(ss);

  // Initialize device arrays
  init_arrays(a);

  std::vector<std::string> labels;
  std::vector<size_t> sizes;
  std::vector<ankerl::nanobench::Bench> benchs;
  int result;

  benchs = {
    // nanobench bsearch
    ankerl::nanobench::Bench().minEpochIterations(10).epochs(num_times).run("binarySearch", [&]{
      for(T x = 0; x<array_size; x++){
        result = binarySearch(a, x, 0, array_size-1);
        if(!BENCHMARK)verify(x, result);
      }
    }),
    ankerl::nanobench::Bench().minEpochIterations(10).epochs(num_times).run("binarySearchRecursive", [&]{
      for(T x = 0; x<array_size; x++){
        result = binarySearchRecursive(a, x,0, array_size-1);
        if(!BENCHMARK)verify(x, result);
      }
    })
  };

  labels = {"Bsearch Iterative", "Bsearch Recursive"};
  sizes = {
      sizeof(T) * ARRAY_SIZE,
      sizeof(T) * ARRAY_SIZE
      };
    
  for (size_t i = 0; i < benchs.size(); ++i)
  {

    // Retrieving nanobench results
    std::vector<ankerl::nanobench::Result> vres;
    vres = benchs[i].results();
    double cyc_op_mean          = vres.begin()->average(ankerl::nanobench::Result::Measure::cpucycles);
    double cyc_op_med           = vres.begin()->median(ankerl::nanobench::Result::Measure::cpucycles);
    double cyc_op_max           = vres.begin()->minimum(ankerl::nanobench::Result::Measure::cpucycles);
    double cyc_op_min           = vres.begin()->maximum(ankerl::nanobench::Result::Measure::cpucycles);
    double cyc_op_err           = vres.begin()->medianAbsolutePercentError(ankerl::nanobench::Result::Measure::cpucycles);

    // writing measures in csv
    if(BENCHMARK){
      res_nano << labels[i] << ";"
      << sizes[i] << ";"
      << cyc_op_mean << ";" 
      << cyc_op_med << ";" 
      << cyc_op_min << ";"
      << cyc_op_max << ";"
      << cyc_op_err << "\n";
    }
  }
  // Add a blank line
  std::cout << std::endl;

  free(a);
}

template<class T>
void Benchmarking()
{
  // CSV open
  if(sizeof(T) == sizeof(float))
  {
    res_nano.open("../results/Benchmark_std_nano_float.csv");
  }
  else 
  {
    res_nano.open("../results/Benchmark_std_nano_double.csv");
  }

  // CSV header
  res_nano << "Function;Size(Bytes);Mean Nano(Cycles);Median Nano(Cycles);Min Nano(Cycles);Max Nano(Cycles);Err Nano(Cycles)\n";

  // CSV data
  for(long long s = 2;  s<pow(2, MAX_SIZE); s=round(s*1.41)){
    ARRAY_SIZE = s;
    run<T>();
  }

  // CSV close
  res_nano.close();
}