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

// Nanobench
#define ANKERL_NANOBENCH_IMPLEMENT
#include "../../../nanobench.h"

// scalar constanst for the mul, triad and nstream kernels
#define SCALAR (0.4)

// Default size of 2^25
int ARRAY_SIZE = 33554432;
int SIZE_X = 3;
int SIZE_Y = 4;
size_t s_align = 1024;
// MHz
int Freq_CPU = 3800;
// Benchmarking
bool BENCHMARK = false;
bool UNITTEST = false;
unsigned int num_times = 100;

std::ofstream res_nano;
std::ofstream res_chrono;

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
    a[i] = (T)(rand()%10)/10;
    b[i] = (T)(rand()%10)/10;
    c[i] = (T)(rand()%10)/10;
  }
}

template <class T>
void init_matrix(
    T *__restrict a,
    T *__restrict b,
    T *__restrict c,
    T initA, T initB, T initC)
{
  const int array_size = ARRAY_SIZE;
  for (int j = 0; j < array_size; j++)
  {
    for (int i = 0; i < array_size; i++)
    {
      a[j*array_size+i] = (T)(rand()%10)/10;
      b[j*array_size+i] = (T)(rand()%10)/10;
      c[j*array_size+i] = (T)(rand()%10)/10;
    }
  }
}

template <class T>
void GEMV(T *__restrict a, T *__restrict b, T *__restrict c)
{
  const int size_x = SIZE_X;
  const int size_y = SIZE_Y;
  for (int j = 0; j < size_y; j++)
  {
    for (int i = 0; i < size_x; i++)
    {
      a[j] += b[j*size_x+i]*c[i];
    }
  }
}

template <class T>
void GEMM(T *__restrict a, T *__restrict b, T *__restrict c)
{
  const int size_x = SIZE_X;
  const int size_y = SIZE_Y;
  for (int j = 0; j < size_y; j++)
  {
    for (int i = 0; i < size_y; i++)
    {
      for (int k = 0; k < size_x; k++)
      {
        a[j*size_y+i] += b[j*size_x+k]*c[k*size_y+i];
      }
    }
  }
}
template <typename T>
void UnitTest(){
  const int size_x = SIZE_X;
  const int size_y = SIZE_Y;

  T *Va = (T *)aligned_alloc(s_align, sizeof(T) * size_x);
  T *Vc = (T *)aligned_alloc(s_align, sizeof(T) * size_x);

  T *Ma = (T *)aligned_alloc(s_align, sizeof(T) * size_y*size_y);
  T *Mb = (T *)aligned_alloc(s_align, sizeof(T) * size_x*size_y);
  T *Mc = (T *)aligned_alloc(s_align, sizeof(T) * size_x*size_y);

  for(int y =0; y<size_y; y++)
  {
    Va[y] = (T) 0;
    for(int x = 0; x<size_y; x++)
    {
      Ma[y*size_y+x] = (T) 0.0;
    }
  }

  Vc[0] = (T) 1;
  Vc[1] = (T) -10;
  Vc[2] = (T) 100;

  Mb[0] = (T) 1;
  Mb[1] = (T) 2;
  Mb[2] = (T) 3;
  Mb[3] = (T) 1;
  Mb[4] = (T) 2;
  Mb[5] = (T) 3;
  Mb[6] = (T) 1;
  Mb[7] = (T) 2;
  Mb[8] = (T) 3;
  Mb[9] = (T) 1;
  Mb[10] = (T) 2;
  Mb[11] = (T) 3;

  Mc[0] = (T) 1;
  Mc[1] = (T) 2;
  Mc[2] = (T) 3;
  Mc[3] = (T) 4;
  Mc[4] = (T) 1;
  Mc[5] = (T) 2;
  Mc[6] = (T) 3;
  Mc[7] = (T) 4;
  Mc[8] = (T) 1;
  Mc[9] = (T) 2;
  Mc[10] = (T) 3;
  Mc[11] = (T) 4;

  if (sizeof(T) == sizeof(float))
    std::cout << "Precision: float" << std::endl;
  else
    std::cout << "Precision: double" << std::endl;

  GEMM(Ma, Mb, Mc);
  GEMV(Va, Mb, Vc);

  std::cout << "GEMV res : \n";
  for(int y =0; y<size_y; y++)
  {
    std::cout << Va[y] << std::endl;
  }
  std::cout << "\n GEMM res : \n";

  for(int y =0; y<size_y; y++)
  {
    for(int x=0; x<size_y; x++){
      std::cout << Ma[y*size_y+x] << " ";
    }
    std::cout << "\n";
  }
}

// Runs the kernel(s) and prints output.
template <typename T>
void run()
{
  std::streamsize ss = std::cout.precision();

  std::cout << "Running kernels " << num_times << " times" << std::endl;

  const int array_size = ARRAY_SIZE;
  T *Va = (T *)aligned_alloc(s_align, sizeof(T) * array_size);
  T *Vb = (T *)aligned_alloc(s_align, sizeof(T) * array_size);
  T *Vc = (T *)aligned_alloc(s_align, sizeof(T) * array_size);

  T *Ma = (T *)aligned_alloc(s_align, sizeof(T) * array_size*array_size);
  T *Mb = (T *)aligned_alloc(s_align, sizeof(T) * array_size*array_size);
  T *Mc = (T *)aligned_alloc(s_align, sizeof(T) * array_size*array_size);

  if (sizeof(T) == sizeof(float))
    std::cout << "Precision: float" << std::endl;
  else
    std::cout << "Precision: double" << std::endl;

  // MB = 10^6
  std::cout << std::setprecision(1) << std::fixed
            << "Array size: " << ARRAY_SIZE * sizeof(T) * 1.0E-6 << " MB"
            << " (=" << ARRAY_SIZE * sizeof(T) * 1.0E-9 << " GB)" << std::endl;
  std::cout << std::setprecision(1) << std::fixed
            << "Matrix size: " << ARRAY_SIZE * ARRAY_SIZE * sizeof(T) * 1.0E-6 << " MB"
            << " (=" << ARRAY_SIZE * ARRAY_SIZE * sizeof(T) * 1.0E-9 << " GB)" << std::endl;
  std::cout << "Total size: " << 3.0 * ARRAY_SIZE*(ARRAY_SIZE+1) * sizeof(T) * 1.0E-6 << " MB"
            << " (=" << 3.0 * ARRAY_SIZE*(ARRAY_SIZE+1) * sizeof(T) * 1.0E-9 << " GB)" << std::endl;
  std::cout.precision(ss);

  // Initialize device arrays
  init_arrays(Va, Vb, Vc, (T)0.0, (T)1.1, (T)1.2);
  std::cout << "init V" << std::endl;
  init_matrix(Ma, Mb, Mc, (T)0.0, (T)1.1, (T)1.2);
  std::cout << "init M" << std::endl;

  // List of times
  using time_t = std::chrono::duration<double, std::micro>;
  std::vector<std::vector<time_t>> timings(2);

  // Declare timers
  std::chrono::high_resolution_clock::time_point t1, t2;
  T resultat = {};
  // Main loop
  for (unsigned int k = 0; k < num_times; k++)
  {
    // Execute Copy
    t1 = std::chrono::high_resolution_clock::now();
    GEMV(Va, Mb, Vc);
    t2 = std::chrono::high_resolution_clock::now();
    timings[0].push_back(std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(t2 - t1));

    // Execute Mul
    t1 = std::chrono::high_resolution_clock::now();
    GEMM(Ma, Mb, Mc);
    t2 = std::chrono::high_resolution_clock::now();
    timings[1].push_back(std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(t2 - t1));
  }
  
  for(int bidon=0; bidon < array_size; bidon++){
    Vb[bidon] = Va[bidon];
  }

  std::vector<std::string> labels;
  std::vector<size_t> sizes;
  std::vector<ankerl::nanobench::Bench> benchs;

  benchs = {
    // nanobench GEMV
    ankerl::nanobench::Bench().minEpochIterations(10).epochs(num_times).run("GEMV", [&]{
    ankerl::nanobench::doNotOptimizeAway(Va);
    GEMV(Va, Mb, Vc);
    }),
    // nanobench GEMM
    ankerl::nanobench::Bench().minEpochIterations(10).epochs(num_times).run("GEMM", [&]{
    ankerl::nanobench::doNotOptimizeAway(Ma);
    GEMM(Ma, Mb, Mc);
    })
  };

  std::cout << "Resultat bidon : " << Va[ARRAY_SIZE-1] << Ma[ARRAY_SIZE*ARRAY_SIZE-1] << std::endl;

  labels = {"GEMV", "GEMM"};
  sizes = {
      2 * sizeof(T) * ARRAY_SIZE + sizeof(T) * ARRAY_SIZE*ARRAY_SIZE,
      3 * sizeof(T) * ARRAY_SIZE * ARRAY_SIZE};

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

    // Retrieving nanobench results
    std::vector<ankerl::nanobench::Result> vres;
    vres = benchs[i].results();
    double cyc_op_mean          = vres.begin()->average(ankerl::nanobench::Result::Measure::cpucycles);
    double bandwidth_nano_mean  =  ((double) sizes[i]*Freq_CPU/1000)/cyc_op_mean;
    double cyc_op_med           = vres.begin()->median(ankerl::nanobench::Result::Measure::cpucycles);
    double bandwidth_nano_med   =  ((double) sizes[i]*Freq_CPU/1000)/cyc_op_med;
    double cyc_op_max           = vres.begin()->minimum(ankerl::nanobench::Result::Measure::cpucycles);
    double bandwidth_nano_min   =  ((double) sizes[i]*Freq_CPU/1000)/cyc_op_max;
    double cyc_op_min           = vres.begin()->maximum(ankerl::nanobench::Result::Measure::cpucycles);
    double bandwidth_nano_max   =  ((double) sizes[i]*Freq_CPU/1000)/cyc_op_min;
    double cyc_op_err           = vres.begin()->medianAbsolutePercentError(ankerl::nanobench::Result::Measure::cpucycles) ;

    std::cout
        << std::left << std::setw(12) << labels[i]
        << std::left << std::setw(12) << std::setprecision(3) << std::fixed << bandwidth
        << std::left << std::setw(12) << std::setprecision(5) << std::scientific << minmax.first->count()
        << std::left << std::setw(12) << std::setprecision(5) << std::scientific << minmax.second->count()
        << std::left << std::setw(12) << std::setprecision(5) << std::scientific << average
        << std::left << std::setw(12) << std::setprecision(3) << std::fixed << bandwidth_nano_mean
        << std::endl;
    
    // writing measures in csv
    if(BENCHMARK){
      res_nano << labels[i] << ";"
      << sizeof(T) * ARRAY_SIZE << ";"
      << bandwidth << ";"
      << bandwidth_nano_mean << ";" 
      << bandwidth_nano_med << ";" 
      << bandwidth_nano_min << ";"
      << bandwidth_nano_max << ";"
      << cyc_op_err << "\n";

      
      res_chrono << labels[i] << ';' << sizeof(T) * ARRAY_SIZE ;
      std::vector<time_t> chronos = timings[i];

      for (std::vector<time_t>::iterator it = chronos.begin() ; it != chronos.end(); ++it)
      {
        double band = (sizes[i]/(it->count()*1e-6))/(1024*1024*1024);
        res_chrono << ';' << band;
      }
      res_chrono << "\n";

    }
  }
  // Add a blank line
  std::cout << std::endl;

  free(Va);
  free(Vb);
  free(Vc);
  free(Ma);
  free(Mb);
  free(Mc);
}

int parseUInt(const char *str, unsigned int *output);
int parseInt(const char *str, int *output);
void parseArguments(int argc, char *argv[]);