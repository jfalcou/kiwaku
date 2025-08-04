#pragma once

#include <sycl/sycl.hpp>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include <cmath>
#include <sstream>
// #define ANKERL_NANOBENCH_IMPLEMENT
#include "utils/utils.hpp"

#include <type_traits> // Required for std::invoke_result

#define ENABLE_TBB true

#if ENABLE_TBB
  #include <execution> // don't forget the -ltbb compiler flag
#endif

// #define EVE_ENABLE_SYCL false

// CMake variables:
// KIWAKU_BUILD_BENCH
// KIWAKU_BENCH_EVE
// KIWAKU_BENCH_SYCL
// KIWAKU_BENCH_MTHREAD
// KIWAKU_ONLY_BENCH_GPU

#define ENABLE_CHECK true


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

// Compute-bound vs memory-bound
enum bench_type_t {compute, memory, GPU_compute, unknown};
enum mem_type_t {RAM, L2, unknown_mem};
enum device_type_t {cpu, gpu};

enum trigo_function_t {sycl_base, sycl_native, std_base};

std::string trigo_function_to_fname(trigo_function_t const type)
{
  switch (type)
  {
    case sycl_base: return "(sycl::)"; break;
    case sycl_native: return "(sycl::native::)"; break;
    case std_base: return "(std::)"; break;
    default: return "(UNKNOWN)"; break;
  }
}


// Each benchmark file is for a direct comparison.
// Each file should be loaded by the python visualizer
// without requiring any change in python code.
// Each benchmark must therefore contain all the necessary information for the plot;

struct cbench_t
{
  // void set_title(std::string global_name_)    { global_name = global_name_; }
  void set_iterations(std::size_t iter_count) { iterations_count = iter_count; }
  void start( std::string const& fname
            , std::string const& global_name
            , std::string const& measured_variable
            , std::size_t total_number_of_elements_processed
            , bench_type_t type      // memory-bound or compute-bound
            );

  // Legacy
  void start( std::string const& fname
            , std::string const& global_name
            , std::string const& measured_variable
            , std::size_t total_number_of_elements_processed
            );

  
  
  // When a reset function is needed for every iteration
  void run_function(std::string const& name, auto func, auto reset_func); 
  void run_function(std::string const& name, auto func);

  // For small arrays (e.g. contained in the L2 cache)
  void run_function_rpt(std::string const& name, std::size_t const repeat, auto func, auto reset_func);

  void run_function_rpt_bwidth(std::string const& name, std::size_t const repeat, auto func, auto reset_func, double tsize_byte);
  void run_function_ext(std::string const& name, std::size_t const repeat, auto func, auto reset_func, double tsize_byte);

  void run_ext2 ( std::string const name
                , auto func
                , auto reset_func
                , double total_number_of_elements_processed // array length * repetitions
                , double bandwidth_per_element_in_bytes // in bytes
                // e.g. for a transform inplace of float: 4 bytes (input) + 4 bytes (ouput) = 8
                , bench_type_t type      // memory-bound or compute-bound
                , double clock_speed_in_GHz // expressed in GHz
                , device_type_t device_type
                );

  void stop();

private:
  std::size_t iterations_count = 6;
  sutils::global_write_file_t current_file;
  std::size_t version = 3;

};

void cbench_t::start( std::string const& fname
                    , std::string const& global_name
                    , std::string const& measured_variable
                    , std::size_t total_number_of_elements_processed
                    , bench_type_t type      // memory-bound or compute-bound
                    )
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
  current_file << total_number_of_elements_processed << "\n";
  if (type == bench_type_t::compute) current_file << "compute-bound\n";
  if (type == bench_type_t::memory)  current_file << "memory-bound\n";
  if (type == bench_type_t::GPU_compute)  current_file << "GPU-compute-bound\n";
  if (type == bench_type_t::unknown) current_file << "!!UNKNOWN!!-bound\n";
  // std::cout << "First line written to file!\n";
}

// Legacy
void cbench_t::start( std::string const& fname
                    , std::string const& global_name
                    , std::string const& measured_variable
                    , std::size_t total_number_of_elements_processed
                    )
{
  start(fname, global_name, measured_variable, total_number_of_elements_processed, bench_type_t::unknown);
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
  std::cout << "Benchmarking  " << name << " (run_function_rpt_bwidth)   time(Mem Bandwidth GB/s):\n";
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
    std::cout << elapsed << "(" << bandwidthGB << ") " << " " << std::flush; // GB/s
    // std::cout << "(" << r << ") " << std::flush; //  "(" << r << ")" <<
  }
  current_file << "\n";
  std::cout << "  sum_ret(" << sum_ret << ")\n\n";
}




// tsize_byte is the total data size read + written by the algorithm, in bytes
template<typename Func>
void cbench_t::run_ext2 ( std::string const name
                        , Func func
                        , auto reset_func
                        , double total_number_of_elements_processed // array length * repetitions
                        , double bandwidth_per_element_in_bytes // in bytes
                        // e.g. for a transform inplace of float: 4 bytes (input) + 4 bytes (ouput) = 8
                        , bench_type_t type      // memory-bound or compute-bound
                        , double clock_speed_in_GHz // expressed in GHz
                        , device_type_t device_type
                        )
{
  std::string unit = "";
  if (type == bench_type_t::memory)       unit = "Mem Bandwidth GB/s";
  if (type == bench_type_t::compute)      unit = "Cycles per element";
  if (type == bench_type_t::GPU_compute)  unit = "GPU usage"; //  "Elapsed (ms)"  Elements per second (billions)

  std::cout << "Benchmarking  " << name << " (run_ext2)   time(" << unit << "):\n";

  // std::cout << "                clock_speed_in_GHz: " << clock_speed_in_GHz << std::endl;
  // // std::cout << "                         elapsed_s: " << elapsed_s << std::endl;
  // std::cout << "total_number_of_elements_processed: " << total_number_of_elements_processed << std::endl;

  // std::cout << "Benchmarking  " << name << " (run_function_rpt_bwidth)   time(Mem Bandwidth GB/s):\n";
  current_file << name << "\n";
  
  // Already written once by the start function.
  // switch (type)
  // {
  //   case memory : current_file << "memory-bound"  << "\n"; break;
  //   case compute: current_file << "compute-bound" << "\n"; break;
  //   default     : current_file << "!!UNKNOWN!!-bound" << "\n"; break;
  // }
  sutils::chrono_t chrono;
  std::cout << "    ";

  // To stop conversion warnings
  using ReturnType = typename std::invoke_result<Func>::type;

  ReturnType sum_ret = 0;

  std::stringstream times_line;
  for (std::size_t i = 0; i < iterations_count; ++i)
  {
    reset_func(); // not measured by the timer
    chrono.Init();
    sum_ret += func();
    std::size_t elapsed = chrono.ElapsedTimeMS();

    times_line << elapsed << " ";

    // Je ne sauvegarde plus le temps, à la place:
    // Memory-bound : GB/s
    // Compute-bound: nombre de cycles par élément
    // current_file << elapsed << " ";

    double elapsed_s = static_cast<double>(elapsed) / 1000;


    // To keep only one digit after the decimal point
    auto concise = [] (double n) -> double
    {
      return std::round(n * 10) / 10;
    };

    // En pourcentage
    if (type == bench_type_t::GPU_compute)
    {
      double elems_per_second = total_number_of_elements_processed / (elapsed_s * 1000000000.); /// billion element per second
      double max_gpu_capacity_trigo = 445; // billion element a second
      double percent4 = std::round(elems_per_second * 10000. / max_gpu_capacity_trigo);
      std::cout << elapsed << "(" << percent4 / 100 << "%) " << " " << std::flush; // GB/s
      
      // current_file << percent4 << " "; // in 100 * percent
      current_file << elapsed << " "; // in 100 * percent
    }

    // GB/s
    if (type == bench_type_t::memory)
    {
      // double bandwidthGB = std::round(((tsize_byte * repeat) / elapsed_s) / 100000000.) / 10; // 100000000. = 0.1 billion
      // ^^^^^ division par 10^8 et non 10^9 pour avoir une décimale

      double bandwidthGB =
        concise(((total_number_of_elements_processed * bandwidth_per_element_in_bytes) / elapsed_s) / 1000000000.); // 1000000000. = 10^9

      std::cout << elapsed << "(" << bandwidthGB << ") " << " " << std::flush; // GB/s
      current_file << bandwidthGB << " ";
    }

    // Cycle per value
    if (type == bench_type_t::compute)
    {
      // Version validée par Hadrien
      // double frequency_hz = 4.7 * 1000000000.; // 3.4 et 4.9, expérimentalement 4,7 (via perf stat -I 1000 -- ./mon_bench) sur Legend
      // double elapsed_per_repetition_s = elapsed_s / static_cast<double>(repeat);
      
      if (device_type == device_type_t::cpu)
      {
        double cycles_per_elem = concise(clock_speed_in_GHz * 1000000000. * elapsed_s / total_number_of_elements_processed);
        std::cout << elapsed << "(" << cycles_per_elem << ") " << " " << std::flush; // GB/s
        current_file << cycles_per_elem << " ";
      }
      
      // TODO????????????? Je ne sais pas quelle métrique prendre.
      if (device_type == device_type_t::gpu)
      {
        double cycles_per_elem = concise(clock_speed_in_GHz * 1000000000. * elapsed_s / total_number_of_elements_processed);
        std::cout << elapsed << "(" << cycles_per_elem << ") " << " " << std::flush; // GB/s
        current_file << cycles_per_elem << " ";
      }
    }

    

    // Ma version
    // // Cycle per value:
    // double frequency = 4.0 * 1000000000.; // 3.4 et 4.9
    // double elapsed_s = static_cast<double>(elapsed) / 1000.;
    // elem_per_cycle = frequency * elapsed_s / (array_size * repeat);
    // //  * element_size
    // // array_size en élements (i.e. nombre de floats)
    // // Temps pour processer array_size éléments

    // // Cycle per value:
    // double frequency_hz = 4.0 * 1000000000.; // 3.4 et 4.9
    // // double elapsed_s = static_cast<double>(elapsed) / 1000.;
    // double elapsed_per_repetition_s = elapsed_s / repeat;
    // double cycle_per_elem = frequency_hz * elapsed_per_repetition_s / array_size;
    


    // std::cout << elapsed << "(" << cycle_per_elem << ") " << " " << std::flush; // GB/s
    // std::cout << "(" << r << ") " << std::flush; //  "(" << r << ")" <<
  }
  current_file << "\n";
  current_file << "Elapsed (ms):\n";
  current_file << times_line.str() << "\n";
  std::cout << "  sum_ret(" << sum_ret << ")\n\n";
}






void cbench_t::run_function_ext ( std::string const& name
                                , std::size_t const repeat
                                , auto func
                                , auto reset_func
                                , double tsize_byte
                                )
{
  std::cout << "Benchmarking  " << name << " (run_function_ext)   time(Mem Bandwidth GB/s):\n";
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
    std::cout << elapsed << "(" << bandwidthGB << ") " << " " << std::flush; // GB/s
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