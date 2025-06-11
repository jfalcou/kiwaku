#pragma once

#include <vector>
#define ANKERL_NANOBENCH_IMPLEMENT
#include <benchmark/utils/utils.hpp>

namespace kwk::bench
{

// Each benchmark file is for a direct comparison.
// Each file should be loaded by the python visualizer
// without requiring any change in python code.
// Each benchmark must therefore contain all the necessary information for the plot;

struct cbench_t
{
  // void set_title(std::string global_name_)    { global_name = global_name_; }
  void set_iterations(std::size_t iter_count) { iterations_count = iter_count; }
  void start(std::string const& fname, std::string const& global_name, std::string const& measured_variable, std::size_t array_size);
  void run_function(std::string const& name, auto func);
  void stop();

private:
  std::size_t iterations_count = 10;
  sutils::global_write_file_t current_file;
  std::size_t version = 3;

};

void cbench_t::start(std::string const& fname, std::string const& global_name, std::string const& measured_variable, std::size_t array_size)
{
  std::cout << "\n============================"
            << "\nStart benchmark environment:\n"
            << "    " << global_name << "\n"
            << "    " << fname << "\n\n";
  current_file.open(fname);
  // std::cout << "File opened!\n";
  current_file << version << "\n";
  current_file << global_name << "\n";
  current_file << measured_variable << "\n";
  current_file << array_size << "\n";
  // std::cout << "First line written to file!\n";
}

void cbench_t::run_function(std::string const& name, auto func)
{
  std::cout << "Benchmarking  " << name << ":\n";
  current_file << name << "\n";
  sutils::chrono_t chrono;
  std::cout << "    ";
  double sum_ret = 0;
  for (std::size_t i = 0; i < iterations_count; ++i)
  {
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

void cbench_t::stop()
{
  current_file.close();
}

std::string fprefix()
{
  return sutils::get_host_name() + "_" + sutils::get_full_date() + "_";
}


}