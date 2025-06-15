#pragma once

#include <vector>
#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"

namespace kwk::bench
{

struct bench_t
{
  ankerl::nanobench::Bench& get() { return b; }
  // void set_iterations(std::size_t i) { b.minEpochIterations(i); }

private:
  ankerl::nanobench::Bench b;
};


bench_t create()
{
  bench_t b;
  b.get().title("Benchmark name");
  return b;
}

void add(bench_t& b, auto func, std::string bench_name, int minIterations)
{
  auto func_internal = [&]()
  {
    func();
    // Remember to always use for unused variables:
    //  ankerl::nanobench::doNotOptimizeAway(variable_name);
  };

  b.get().performanceCounters(true);
  if (minIterations > 0) b.get().run(bench_name, func_internal).minEpochIterations(10);
  else                   b.get().run(bench_name, func_internal);
}

void add(bench_t& b, auto func, std::string bench_name)
{
  add(b, func, bench_name, -1);
}

void execute(bench_t& b)
{
  std::vector<ankerl::nanobench::Result> vres = b.get().results();

  for (auto& e : vres)
  {
    std::cout << "\n";
    // Elapsed, per iteration
    std::cout << "elapsed........... " << e.median(ankerl::nanobench::Result::Measure::elapsed) << "\n";
    std::cout << "iterations........ " << e.median(ankerl::nanobench::Result::Measure::iterations) << "\n";
    std::cout << "\n";

    // Requires sudo privileges
    std::cout << "pagefaults........ " << e.median(ankerl::nanobench::Result::Measure::pagefaults) << "\n";
    std::cout << "cpucycles......... " << e.median(ankerl::nanobench::Result::Measure::cpucycles) << "\n";
    std::cout << "contextswitches... " << e.median(ankerl::nanobench::Result::Measure::contextswitches) << "\n";
    std::cout << "instructions...... " << e.median(ankerl::nanobench::Result::Measure::instructions) << "\n";
    std::cout << "branchinstructions " << e.median(ankerl::nanobench::Result::Measure::branchinstructions) << "\n";
    std::cout << "branchmisses...... " << e.median(ankerl::nanobench::Result::Measure::branchmisses) << "\n";
    std::cout << "\n";
  }

  // // double cyc_op_med           =   vres.begin()->median(ankerl::nanobench::Result::Measure::cpucycles);
  // double cyc_op_mean          =   vres.begin()->median(ankerl::nanobench::Result::Measure::cpucycles); // Nombre cycles total
  // double time_mean            =   vres.begin()->median(ankerl::nanobench::Result::Measure::elapsed); // Nombre cycles total
  // // double cyc_op_max           =   vres.begin()->maximum(ankerl::nanobench::Result::Measure::cpucycles);
  // // double cyc_op_min           =   vres.begin()->minimum(ankerl::nanobench::Result::Measure::cpucycles);

  // std::cout << "cyc_op_mean = " << cyc_op_mean << "\n";
  // std::cout << "time_mean   = " << time_mean << "\n";
  // // std::cout << "repeated    = " << repeated << "\n";
}


}