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

// Nanobench
#define ANKERL_NANOBENCH_IMPLEMENT
#include "../../nanobench.h"

// scalar constanst for the mul, triad and nstream kernels
#define SCALAR (0.4)

// Default size of 2^25
int ARRAY_SIZE = 33554432;
// MHz
int Freq_CPU = 3800;
unsigned int num_times = 100;

std::ofstream myresults;

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

template <class T>
void copy(const T *__restrict a, T *__restrict c)
{
  const int array_size = ARRAY_SIZE;
  for (int i = 0; i < array_size; i++)
    c[i] = a[i];
}

template <class T>
void mul(T *__restrict b, const T *__restrict c)
{
  const int array_size = ARRAY_SIZE;
  for (int i = 0; i < array_size; i++)
  {
    const T scalar = SCALAR;
    b[i] = scalar * c[i];
  }
}

template <class T>
void add(const T *__restrict a, const T *__restrict b, T *__restrict c)
{
  const int array_size = ARRAY_SIZE;
  for (int i = 0; i < array_size; i++)
  {
    c[i] = a[i] + b[i];
  }
}

template <class T>
void triad(T *__restrict a, const T *__restrict b, const T *__restrict c)
{
  const int array_size = ARRAY_SIZE;
  for (int i = 0; i < array_size; i++)
  {
    const T scalar = SCALAR;
    a[i] = b[i] + scalar * c[i];
  }
}

template <class T>
void nstream(T *__restrict a, const T *__restrict b, const T *__restrict c)
{
  const int array_size = ARRAY_SIZE;
  for (int i = 0; i < array_size; i++)
  {
    const T scalar = SCALAR;
    a[i] += b[i] + scalar * c[i];
  }
}

template <class T>
T dot(const T *__restrict a, const T *__restrict b)
{
  const int array_size = ARRAY_SIZE;
  T sum = 0.0;
  for (int i = 0; i < array_size; i++)
  {
    sum += a[i] * b[i];
  }
  return sum;
}

// Runs the kernel(s) and prints output.
template <typename T>
void run()
{
  std::streamsize ss = std::cout.precision();

  std::cout << "Running kernels " << num_times << " times" << std::endl;

  const int array_size = ARRAY_SIZE;
  T *a = (T *)aligned_alloc(1024, sizeof(T) * array_size);
  T *b = (T *)aligned_alloc(1024, sizeof(T) * array_size);
  T *c = (T *)aligned_alloc(1024, sizeof(T) * array_size);

  if (sizeof(T) == sizeof(float))
    std::cout << "Precision: float" << std::endl;
  else
    std::cout << "Precision: double" << std::endl;

  // MB = 10^6
  std::cout << std::setprecision(1) << std::fixed
            << "Array size: " << ARRAY_SIZE * sizeof(T) * 1.0E-6 << " MB"
            << " (=" << ARRAY_SIZE * sizeof(T) * 1.0E-9 << " GB)" << std::endl;
  std::cout << "Total size: " << 3.0 * ARRAY_SIZE * sizeof(T) * 1.0E-6 << " MB"
            << " (=" << 3.0 * ARRAY_SIZE * sizeof(T) * 1.0E-9 << " GB)" << std::endl;
  std::cout.precision(ss);

  // Initialize device arrays
  init_arrays(a, b, c, (T)0.1, (T)0.2, T(0.0));

  // List of times
  using time_t = std::chrono::duration<double, std::micro>;
  std::vector<std::vector<time_t>> timings(6);

  // Declare timers
  std::chrono::high_resolution_clock::time_point t1, t2;
  T resultat = {};
  // Main loop
  for (unsigned int k = 0; k < num_times; k++)
  {
    // Execute Copy
    t1 = std::chrono::high_resolution_clock::now();
    copy(a, c);
    t2 = std::chrono::high_resolution_clock::now();
    timings[0].push_back(std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(t2 - t1));

    // Execute Mul
    t1 = std::chrono::high_resolution_clock::now();
    mul(b, c);
    t2 = std::chrono::high_resolution_clock::now();
    timings[1].push_back(std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(t2 - t1));

    // Execute Add
    t1 = std::chrono::high_resolution_clock::now();
    add(a, b, c);
    t2 = std::chrono::high_resolution_clock::now();
    timings[2].push_back(std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(t2 - t1));

    // Execute Triad
    t1 = std::chrono::high_resolution_clock::now();
    triad(a, b, c);
    t2 = std::chrono::high_resolution_clock::now();
    timings[3].push_back(std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(t2 - t1));

    // Execute Dot
    t1 = std::chrono::high_resolution_clock::now();
    resultat = dot(a, b);
    t2 = std::chrono::high_resolution_clock::now();

    timings[4].push_back(std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(t2 - t1));

    // Execute NStream
    t1 = std::chrono::high_resolution_clock::now();
    nstream(a, b, c);
    t2 = std::chrono::high_resolution_clock::now();
    timings[5].push_back(std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(t2 - t1));
  }

  std::vector<std::string> labels;
  std::vector<size_t> sizes;
  std::vector<ankerl::nanobench::Bench> benchs;

  benchs = {
    // nanobench Copy
    ankerl::nanobench::Bench().minEpochIterations(10).epochs(num_times).run("Copy", [&]{
    copy(a, c);
    }),
    // nanobench Mul
    ankerl::nanobench::Bench().minEpochIterations(10).epochs(num_times).run("Mul", [&]{
    mul(b, c);
    }),
    // nanobench Add
    ankerl::nanobench::Bench().minEpochIterations(10).epochs(num_times).run("Add", [&]{
    add(a, b, c);
    }),
    // nanobench Triad
    ankerl::nanobench::Bench().minEpochIterations(10).epochs(num_times).run("Triad", [&]{
    triad(a, b, c);
    }),
    // nanobench Dot
    ankerl::nanobench::Bench().minEpochIterations(10).epochs(num_times).run("Dot", [&]{
      resultat = dot(a, b);
    }),
    // nanobench NStream
    ankerl::nanobench::Bench().minEpochIterations(10).epochs(num_times).run("NStream", [&]{
      nstream(a, b, c);
    })
  };

  std::cout << "Resultat bidon : " << resultat << std::endl;

  labels = {"Copy", "Mul", "Add", "Triad", "Dot", "Nstream"};
  sizes = {
      2 * sizeof(T) * ARRAY_SIZE,
      2 * sizeof(T) * ARRAY_SIZE,
      3 * sizeof(T) * ARRAY_SIZE,
      3 * sizeof(T) * ARRAY_SIZE,
      2 * sizeof(T) * ARRAY_SIZE,
      4 * sizeof(T) * ARRAY_SIZE};

  // Display timing results
  std::cout
      << std::left << std::setw(12) << "Function"
      << std::left << std::setw(12) << "GBytes/sec"
      << std::left << std::setw(12) << "Min (usec)"
      << std::left << std::setw(12) << "Max"
      << std::left << std::setw(12) << "Average"
      << std::left << std::setw(12) << "Nanobench GBytes/sec"
      << std::endl
      << std::fixed;

    
  for (size_t i = 0; i < timings.size(); ++i)
  {
    // Get min/max; ignore the first result
    auto minmax = std::minmax_element(timings[i].begin() + 1, timings[i].end());

    // Calculate average; ignore the first result
    double average = std::accumulate(timings[i].begin() + 1, timings[i].end(), 0.0, [](auto acc, auto r)
                                     { return acc + r.count(); }) /
                     (double)(num_times - 1);

    double bandwidth = (sizes[i] / (minmax.first->count() * 1e-6))/(1024*1024*1024);

    std::vector<ankerl::nanobench::Result> vres;
    vres = benchs[i].results();
    double cyc_op_mean = vres.begin()->average(ankerl::nanobench::Result::Measure::cpucycles);
    double bandwidth_nano_mean =  (sizes[i]*Freq_CPU/1000)/cyc_op_mean;
    double cyc_op_med = vres.begin()->median(ankerl::nanobench::Result::Measure::cpucycles);
    double bandwidth_nano_med =  (sizes[i]*Freq_CPU/1000)/cyc_op_med;
    double cyc_op_min = vres.begin()->minimum(ankerl::nanobench::Result::Measure::cpucycles);
    double bandwidth_nano_min =  (sizes[i]*Freq_CPU/1000)/cyc_op_min;
    double cyc_op_max = vres.begin()->maximum(ankerl::nanobench::Result::Measure::cpucycles);
    double bandwidth_nano_max =  (sizes[i]*Freq_CPU/1000)/cyc_op_max;
    double cyc_op_err = vres.begin()->medianAbsolutePercentError(ankerl::nanobench::Result::Measure::cpucycles);

    std::cout
        << std::left << std::setw(12) << labels[i]
        << std::left << std::setw(12) << std::setprecision(3) << std::fixed << bandwidth
        << std::left << std::setw(12) << std::setprecision(5) << std::scientific << minmax.first->count()
        << std::left << std::setw(12) << std::setprecision(5) << std::scientific << minmax.second->count()
        << std::left << std::setw(12) << std::setprecision(5) << std::scientific << average
        << std::left << std::setw(12) << std::setprecision(3) << std::fixed << bandwidth_nano_mean
        << std::endl;
    
    myresults << labels[i] << ";"
    << sizeof(T) * ARRAY_SIZE << ";"
    << bandwidth << ";"
    << bandwidth_nano_mean << ";" 
    << bandwidth_nano_mean << ";" 
    << bandwidth_nano_min << ";"
    << bandwidth_nano_max << ";"
    << cyc_op_err << ";\n";
  }
  // Add a blank line
  std::cout << std::endl;

  free(a);
  free(b);
  free(c);
}

int parseUInt(const char *str, unsigned int *output);
int parseInt(const char *str, int *output);
void parseArguments(int argc, char *argv[]);