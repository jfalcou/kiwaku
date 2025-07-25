#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include <cmath>
// #define ANKERL_NANOBENCH_IMPLEMENT
#include "utils/utils.hpp"

#define ENABLE_TBB true

#if ENABLE_TBB
  #include <execution> // don't forget the -ltbb compiler flag
#endif

#define EVE_ENABLE_SYCL false


namespace kwk::bench
{



// const double LEGEND_LOAD_FACTOR = 0.001;
// const double LEGEND_LOAD_FACTOR = 0.2;
const double LEGEND_LOAD_FACTOR = 1.;

bool enable_global = true;

std::string EVE_COMPILER_FLAG = "ERROR-unknown-EVE-flag";

// Will be updated in get_eve_compiler_flag()
std::string EVE_BACKEND_NAME  = "Kiwaku SIMD " + EVE_COMPILER_FLAG; 

// -mavx2 -mfma
// -msse4.2 on devrait avoir x4
// -march=skylake-avx512

// Each benchmark file is for a direct comparison.
// Each file should be loaded by the python visualizer
// without requiring any change in python code.
// Each benchmark must therefore contain all the necessary information for the plot;

struct cbench_t
{
  // void set_title(std::string global_name_)    { global_name = global_name_; }
  void set_iterations(std::size_t iter_count) { iterations_count = iter_count; }
  void start(std::string const& fname, std::string const& global_name, std::string const& measured_variable, std::size_t array_size);

  
  
  // When a reset function is needed for every iteration
  void run_function(std::string const& name, auto func, auto reset_func); 
  void run_function(std::string const& name, auto func);

  // For small arrays (e.g. contained in the L2 cache)
  void run_function_rpt(std::string const& name, std::size_t const repeat, auto func, auto reset_func);

  void run_function_rpt_bwidth(std::string const& name, std::size_t const repeat, auto func, auto reset_func, double tsize_byte);

  void stop();

private:
  std::size_t iterations_count = 4;
  sutils::global_write_file_t current_file;
  std::size_t version = 3;

};

void cbench_t::start(std::string const& fname, std::string const& global_name, std::string const& measured_variable, std::size_t array_size)
{
  std::string out_dir = "../kiwaku_bench_results/";

  std::cout << "\n============================"
            << "\nStart benchmark environment:\n"
            << "    " << global_name << "\n"
            << "    " << fname << "\n"
            << "    in directory " << out_dir << "\n\n";
  
  std::filesystem::create_directory(out_dir);

  current_file.open(out_dir + fname);
  // std::cout << "File opened!\n";
  current_file << version << "\n";
  current_file << global_name << "\n";
  current_file << measured_variable << "\n";
  current_file << array_size << "\n";
  // std::cout << "First line written to file!\n";
}


void get_eve_compiler_flag()
{
  if (const char* env_p = std::getenv("EVE_FLAG"))
  {
    EVE_COMPILER_FLAG = std::string(env_p);
    std::replace(EVE_COMPILER_FLAG.begin(), EVE_COMPILER_FLAG.end(), ' ', '_'); // replace all ' ' by '_'
    EVE_BACKEND_NAME = "Kiwaku SIMD " + EVE_COMPILER_FLAG;
    std::cout << "EVE_COMPILER_FLAG set to " << EVE_COMPILER_FLAG << "\n";
  } else {
    std::cout << "\n\n";
    std::cout << "=======================================================\n";
    std::cout << "ERROR: please define the EVE_FLAG environment variable.\n";
    std::cout << "=======================================================\n\n\n";
    std::terminate();
  }
}


void cbench_t::run_function(std::string const& name, auto func, auto reset_func)
{
  std::cout << "Benchmarking  " << name << ":\n";
  current_file << name << "\n";
  sutils::chrono_t chrono;
  std::cout << "    ";
  double sum_ret = 0;
  for (std::size_t i = 0; i < iterations_count; ++i)
  {
    reset_func(); // not measured by the timer
    chrono.Init();
    auto r = func();
    std::size_t elapsed = chrono.ElapsedTimeMS() ;
    current_file << elapsed << " ";
    sum_ret += r;
    std::cout << elapsed << " " << std::flush;
    // std::cout << "(" << r << ") " << std::flush; //  "(" << r << ")" <<
  }
  current_file << "\n";
  std::cout << "  sum_ret(" << sum_ret << ")\n\n";
}
void cbench_t::run_function(std::string const& name, auto func)
{ 
  auto reset_func = []{ /* Do nothing */ };
  run_function(name, func, reset_func);
}


void cbench_t::run_function_rpt(std::string const& name, std::size_t const repeat, auto func, auto reset_func)
{
  std::cout << "Benchmarking  " << name << ":\n";
  current_file << name << "\n";
  sutils::chrono_t chrono;
  std::cout << "    ";
  double sum_ret = 0;
  for (std::size_t i = 0; i < iterations_count; ++i)
  {
    reset_func(); // not measured by the timer
    chrono.Init();
    for (std::size_t i2 = 0; i2 < repeat; ++i2)
    {
      auto r = func();
      sum_ret += r;
    }
    std::size_t elapsed = chrono.ElapsedTimeMS() ;
    current_file << elapsed << " ";
    std::cout << elapsed << " " << std::flush;
    // std::cout << "(" << r << ") " << std::flush; //  "(" << r << ")" <<
  }
  current_file << "\n";
  std::cout << "  sum_ret(" << sum_ret << ")\n\n";
}

// tsize_byte is the total data size read + written by the algorithm, in bytes
void cbench_t::run_function_rpt_bwidth(std::string const& name, std::size_t const repeat, auto func, auto reset_func, double tsize_byte)
{
  std::cout << "Benchmarking  " << name << " (run_function_rpt_bwidth):\n";
  current_file << name << "\n";
  sutils::chrono_t chrono;
  std::cout << "    ";
  double sum_ret = 0;
  for (std::size_t i = 0; i < iterations_count; ++i)
  {
    reset_func(); // not measured by the timer
    chrono.Init();
    for (std::size_t i2 = 0; i2 < repeat; ++i2)
    {
      auto r = func();
      sum_ret += r;
    }
    std::size_t elapsed = chrono.ElapsedTimeMS() ;
    double elapsed_s = static_cast<double>(elapsed) / 1000;
    double bandwidthGB = std::round((tsize_byte / elapsed_s) / 100000000.) / 10; // 100000000. = 0.1 billion

    current_file << elapsed << " ";
    std::cout << elapsed << "(" << bandwidthGB << "GB/s) " << " " << std::flush;
    // std::cout << "(" << r << ") " << std::flush; //  "(" << r << ")" <<
  }
  current_file << "\n";
  std::cout << "  sum_ret(" << sum_ret << ")\n\n";
}

void cbench_t::stop()
{
  current_file.close();
}

std::string fprefix()
{
  return sutils::get_host_name() + "_" + sutils::get_full_date() + "_";
}

template<typename DATA_TYPE>
DATA_TYPE random_float(DATA_TYPE min, DATA_TYPE max)
{
  return (static_cast<DATA_TYPE>(rand()) / static_cast<DATA_TYPE>(RAND_MAX)) * (max - min) + min; 
}

}