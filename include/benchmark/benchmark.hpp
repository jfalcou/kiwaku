#pragma once

#include <vector>
#define ANKERL_NANOBENCH_IMPLEMENT
#include <benchmark/utils/utils.hpp>

namespace kwk::bench
{

// Each benchmark file is for a direct comparison
// Each file should be loaded by the python visualizer
// with requiring any change in python code.

// Each benchmark must therefore contain all the necessary information for the plot
// 

// File composition:

// Global benchmark title
// And for each benchmark:
//   Benchmark name
//   list of durations on one single line, separated by spaces
//   ...


struct cbench_t
{
  // void set_title(std::string global_name_)    { global_name = global_name_; }
  void set_iterations(std::size_t iter_count) { iterations_count = iter_count; }
  void start(std::string const& fname, std::string const& global_name);
  void run_function(std::string const& name, auto func);
  void stop();

private:
  std::size_t iterations_count = 10;
  global_write_file_t current_file;

};

void cbench_t::start(std::string const& fname, std::string const& global_name)
{
  std::cout << "Start benchmark environment: " << global_name << "\n";
  current_file.open(fname);
  // std::cout << "File opened!\n";
  current_file << global_name << "\n";
  // std::cout << "First line written to file!\n";
}

void cbench_t::run_function(std::string const& name, auto func)
{
  std::cout << "Benchmarking... " << name << "\n";
  current_file << name << "\n";
  ::bench::chrono_t chrono;
  for (std::size_t i = 0; i < iterations_count; ++i)
  {
    chrono.Init();
    auto r = func();
    std::size_t elapsed = chrono.ElapsedTimeMS() ;
    current_file << elapsed << " ";
    std::cout << "(" << r << ")-" << elapsed << "ms ";
  }
  std::cout << "\n";
}

void cbench_t::stop()
{
  current_file.close();
}


}