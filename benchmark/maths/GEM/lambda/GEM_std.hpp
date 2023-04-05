#include "../GEM.hpp"
#include <cblas.h>
#include <ctime>

// Bench
#define ANKERL_NANOBENCH_IMPLEMENT
#include "../../../nanobench.h"

// Files
std::ofstream res_nano;
std::ofstream res_chrono;

// Default size of 32
int ARRAY_SIZE = 32;
int SIZE_X = 3;
int SIZE_Y = 4;
size_t s_align = 1024;

// MHz
int Freq_CPU = 3800;

// Benchmarking
bool BENCHMARK = false;
bool UNITTEST = false;
unsigned int num_times = 100;

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

template <class T>
void GEMMSmart(T *__restrict a, T *__restrict b, T *__restrict c)
{
  const int size_x = SIZE_X;
  const int size_y = SIZE_Y;
  
  for (int j = 0; j < size_y; j++)
  {
    for (int k = 0; k < size_x; k++)
    {
      for (int i = 0; i < size_y; i++)
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
  const int array_size = ARRAY_SIZE;

  T *Va = (T *)aligned_alloc(s_align, sizeof(T) * size_y);
  T *Vc = (T *)aligned_alloc(s_align, sizeof(T) * size_x);

  T *Ma = (T *)aligned_alloc(s_align, sizeof(T) * size_y*size_y);
  T *Mb = (T *)aligned_alloc(s_align, sizeof(T) * size_y*size_x);
  T *Mc = (T *)aligned_alloc(s_align, sizeof(T) * size_x*size_y);


  for(int y =0; y<size_y; y++)
  {
    Vc[y] = T(10*y-1);
    for(int x = 0; x<size_x; x++)
    {
      Mb[y*size_x+x] = T(1+x);
      Mc[x*size_y+y] = T(1+y);
    }
  }

  if (sizeof(T) == sizeof(float))
    std::cout << "Precision: float" << std::endl;
  else
    std::cout << "Precision: double" << std::endl;

  GEMV<T>(Va, Mb, Vc);
  std::cout << "GEMV res : \n";
  for(int y =0; y<size_y; y++)
  {
    std::cout << Va[y] << std::endl;
  }

  GEMM<T>(Ma, Mb, Mc);
  std::cout << "\n GEMM res : \n";
  for(int y =0; y<size_y; y++)
  {
    for(int x=0; x<size_y; x++){
      std::cout << Ma[y*size_y+x] << " ";
    }
    std::cout << "\n";
  }

  GEMMSmart<T>(Ma, Mb, Mc);
  std::cout << "\n GEMMSmart res : \n";
  for(int y =0; y<size_y; y++)
  {
    for(int x=0; x<size_y; x++){
      std::cout << Ma[y*size_y+x] << " ";
    }
    std::cout << "\n";
  }

  free(Va);
  free(Vc);
  free(Ma);
  free(Mb);
  free(Mc);
}

// Runs the kernel(s) and prints output.
template <typename T>
void run()
{
  // Data
  const int array_size = ARRAY_SIZE;
  T *Va = (T *)aligned_alloc(s_align, sizeof(T) * array_size);
  T *Vc = (T *)aligned_alloc(s_align, sizeof(T) * array_size);

  T *Ma = (T *)aligned_alloc(s_align, sizeof(T) * array_size*array_size);
  T *Mb = (T *)aligned_alloc(s_align, sizeof(T) * array_size*array_size);
  T *Mc = (T *)aligned_alloc(s_align, sizeof(T) * array_size*array_size);


  std::vector<std::string> labels;
  std::vector<size_t> sizes;
  std::vector<ankerl::nanobench::Bench> benchs;

  // Functions and space use
  labels = {"GEMV", "GEMM", "GEMMSmart", "BLAS GEMV", "BLAS GEMM"};
  sizes = {
      sizeof(T) * ARRAY_SIZE + 2 * sizeof(T) * ARRAY_SIZE * ARRAY_SIZE ,
      sizeof(T) * ARRAY_SIZE * ARRAY_SIZE + 2 * sizeof(T) * ARRAY_SIZE * ARRAY_SIZE * ARRAY_SIZE,
      sizeof(T) * ARRAY_SIZE * ARRAY_SIZE + 2 * sizeof(T) * ARRAY_SIZE * ARRAY_SIZE * ARRAY_SIZE,
      sizeof(T) * ARRAY_SIZE + 2 * sizeof(T) * ARRAY_SIZE * ARRAY_SIZE ,
      sizeof(T) * ARRAY_SIZE * ARRAY_SIZE + 2 * sizeof(T) * ARRAY_SIZE * ARRAY_SIZE * ARRAY_SIZE};

  // List of times
  using time_t = std::chrono::duration<double, std::micro>;
  std::vector<std::vector<time_t>> timings(5);

  // Declare timers
  std::chrono::high_resolution_clock::time_point t1, t2;

  print_init<T>(sizes, 5);

  // Initialize device arrays
  init_arrays(Va, Vc);
  std::cout << "init V" << std::endl;
  init_matrix(Ma, Mb, Mc);
  std::cout << "init M" << std::endl;

  // Main loop
  for (unsigned int k = 0; k < num_times; k++)
  {
    // Execute GEMV
    t1 = std::chrono::high_resolution_clock::now();
    GEMV<T>(Va, Mb, Vc);
    t2 = std::chrono::high_resolution_clock::now();
    timings[0].push_back(std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(t2 - t1));

    // Execute GEMM
    t1 = std::chrono::high_resolution_clock::now();
    GEMM<T>(Ma, Mb, Mc);
    t2 = std::chrono::high_resolution_clock::now();
    timings[1].push_back(std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(t2 - t1));

    // Execute GEMMSmart
    t1 = std::chrono::high_resolution_clock::now();
    GEMMSmart<T>(Ma, Mb, Mc);
    t2 = std::chrono::high_resolution_clock::now();
    timings[2].push_back(std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(t2 - t1));
  
    if constexpr(sizeof(T) == sizeof(float))
    {
      t1 = std::chrono::high_resolution_clock::now();
      cblas_sgemv(CblasRowMajor, CblasNoTrans, array_size, array_size, 1.0, Mb, array_size, Vc, 1, 0.0, Vc, 1);
      t2 = std::chrono::high_resolution_clock::now();
      timings[3].push_back(std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(t2 - t1));

      t1 = std::chrono::high_resolution_clock::now();
      cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, array_size, array_size, array_size, 1.0, Mb, array_size, Mc, array_size, 0.0, Mc, array_size);
      t2 = std::chrono::high_resolution_clock::now();
      timings[4].push_back(std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(t2 - t1));
    } 
    else 
    {
      t1 = std::chrono::high_resolution_clock::now();
      cblas_dgemv(CblasRowMajor, CblasNoTrans, array_size, array_size, 1.0, Mb, array_size, Vc, 1, 0.0, Vc, 1);
      t2 = std::chrono::high_resolution_clock::now();
      timings[3].push_back(std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(t2 - t1));

      t1 = std::chrono::high_resolution_clock::now();
      cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, array_size, array_size, array_size, 1.0, Mb, array_size, Mc, array_size, 0.0, Mc, array_size);
      t2 = std::chrono::high_resolution_clock::now();
      timings[4].push_back(std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(t2 - t1));
    }
  }

  if constexpr(sizeof(T) == sizeof(float))
  {
    benchs = {
    // nanobench GEMV
    ankerl::nanobench::Bench().minEpochIterations(10).epochs(num_times).run("GEMV", [&]{
    ankerl::nanobench::doNotOptimizeAway(Va);
    GEMV<T>(Va, Mb, Vc);
    }),
    // nanobench GEMM
    ankerl::nanobench::Bench().minEpochIterations(10).epochs(num_times).run("GEMM", [&]{
    // ankerl::nanobench::doNotOptimizeAway(Ma);
    GEMM<T>(Ma, Mb, Mc);
    }),
    ankerl::nanobench::Bench().minEpochIterations(10).epochs(num_times).run("GEMMSmart", [&]{
    // ankerl::nanobench::doNotOptimizeAway(Ma);
    GEMMSmart<T>(Ma, Mb, Mc);
    }),
    ankerl::nanobench::Bench().minEpochIterations(10).epochs(num_times).run("GEMV BLAS", [&]{
    // ankerl::nanobench::doNotOptimizeAway(Ma);
    ankerl::nanobench::doNotOptimizeAway(Va);
    cblas_sgemv(CblasRowMajor, CblasNoTrans, array_size, array_size, 1.0, Mb, array_size, Vc, 1, 0.0, Vc, 1);
    }),
    ankerl::nanobench::Bench().minEpochIterations(10).epochs(num_times).run("GEMM BLAS", [&]{
    // ankerl::nanobench::doNotOptimizeAway(Ma);
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, array_size, array_size, array_size, 1.0, Mb, array_size, Mc, array_size, 0.0, Mc, array_size);
    })
    };
  } 
  else
  {
    benchs = {
    // nanobench GEMV
    ankerl::nanobench::Bench().minEpochIterations(10).epochs(num_times).run("GEMV", [&]{
    // ankerl::nanobench::doNotOptimizeAway(Mb);
    ankerl::nanobench::doNotOptimizeAway(Va);
    GEMV<T>(Va, Mb, Vc);
    }),
    // nanobench GEMM
    ankerl::nanobench::Bench().minEpochIterations(10).epochs(num_times).run("GEMM", [&]{
    // ankerl::nanobench::doNotOptimizeAway(Ma);
    GEMM<T>(Ma, Mb, Mc);
    }),
    ankerl::nanobench::Bench().minEpochIterations(10).epochs(num_times).run("GEMMSmart", [&]{
    // ankerl::nanobench::doNotOptimizeAway(Ma);
    GEMMSmart<T>(Ma, Mb, Mc);
    }),
    ankerl::nanobench::Bench().minEpochIterations(10).epochs(num_times).run("GEMV BLAS", [&]{
    // ankerl::nanobench::doNotOptimizeAway(Ma);
    ankerl::nanobench::doNotOptimizeAway(Va);
    cblas_dgemv(CblasRowMajor, CblasNoTrans, array_size, array_size, 1.0, Mb, array_size, Vc, 1, 0.0, Vc, 1);
    }),
    ankerl::nanobench::Bench().minEpochIterations(10).epochs(num_times).run("GEMM BLAS", [&]{
    // ankerl::nanobench::doNotOptimizeAway(Ma);
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, array_size, array_size, array_size, 1.0, Mb, array_size, Mc, array_size, 0.0, Mc, array_size);
    })
    };
  }

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
    double cyc_op_mean          =   vres.begin()->average(ankerl::nanobench::Result::Measure::cpucycles);
    double bandwidth_nano_mean  =  ((double) sizes[i]*Freq_CPU/1000)/cyc_op_mean;
    double cyc_op_med           =   vres.begin()->median(ankerl::nanobench::Result::Measure::cpucycles);
    double bandwidth_nano_med   =  ((double) sizes[i]*Freq_CPU/1000)/cyc_op_med;
    double cyc_op_max           =   vres.begin()->minimum(ankerl::nanobench::Result::Measure::cpucycles);
    double bandwidth_nano_min   =  ((double) sizes[i]*Freq_CPU/1000)/cyc_op_max;
    double cyc_op_min           =   vres.begin()->maximum(ankerl::nanobench::Result::Measure::cpucycles);
    double bandwidth_nano_max   =  ((double) sizes[i]*Freq_CPU/1000)/cyc_op_min;
    double cyc_op_err           =   vres.begin()->medianAbsolutePercentError(ankerl::nanobench::Result::Measure::cpucycles) ;
    double bandwidth_nano_err   =  ((double) sizes[i]*Freq_CPU/1000)/cyc_op_err;

    std::cout
        << std::left << std::setw(12) << labels[i]
        << std::left << std::setw(12) << std::setprecision(3) << std::fixed << bandwidth
        << std::left << std::setw(12) << std::setprecision(5) << std::scientific << minmax.first->count()
        << std::left << std::setw(12) << std::setprecision(5) << std::scientific << minmax.second->count()
        << std::left << std::setw(12) << std::setprecision(5) << std::scientific << average
        << std::left << std::setw(12) << std::setprecision(3) << std::fixed << bandwidth_nano_mean
        << std::endl;
    
    // Writing measures in csv
    if(BENCHMARK){
      res_nano << labels[i] << ";"
      << sizes[i] << ";"
      << bandwidth << ";"
      << bandwidth_nano_mean << ";" 
      << bandwidth_nano_med << ";" 
      << bandwidth_nano_min << ";"
      << bandwidth_nano_max << ";"
      << bandwidth_nano_err << "\n";

      res_chrono << labels[i] << ';' << sizeof(T) * sizes[i] ;
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
  free(Vc);
  free(Ma);
  free(Mb);
  free(Mc);
}

template<class T>
void Benchmarking()
{
  // CSV open
  if(sizeof(T) == sizeof(float))
  {
    res_nano.open("./Benchmark_std_nano_float.csv");
    res_chrono.open("./Benchmark_std_chrono_float.csv");
  }
  else 
  {
    res_nano.open("./Benchmark_std_nano_double.csv");
    res_chrono.open("./Benchmark_std_chrono_double.csv");
  }

  // CSV header
  res_nano << "Function;Size(Bytes);Mean Chrono(GBytes/sec);Mean Nano(GBytes/sec);Median Nano(GBytes/sec);Min Nano(GBytes/sec);Max Nano(GBytes/sec);Err Nano(GBytes/sec);\n";
  res_chrono << "Function;Size(Bytes);";
  for(uint n=0; n<num_times; n++)res_chrono << n << ";";
  res_chrono << "\n";

  // CSV data
  for(long long s = 2;  s<pow(2, 10); s=round(s*1.41)){
    ARRAY_SIZE = s;
    SIZE_X = ARRAY_SIZE;
    SIZE_Y = ARRAY_SIZE;
    run<T>();
  }

  // CSV close
  res_nano.close();
  res_chrono.close();
}