//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/context/sycl/context.hpp>
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/reduce.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <numeric>
#include "include/benchmark.hpp"
#include "include/utils/utils.hpp"
// #include <execution> for parallel std execution



/// @brief 
/// @tparam DATA_TYPE must be a floating-point type
/// @param bench_name 
/// @param file_name 
/// @param func 
/// @param view_size  
template<typename DATA_TYPE>
void reduce_test(std::string const& bench_name, std::string const& file_name, auto func, std::size_t const view_size)
{

  // The maximum rounding-errors between function results
  float ERROR_MAX_PERCENT = 50; // 50% difference max

  const std::size_t d0 = view_size;
  const std::size_t input_size = d0;
  std::vector<DATA_TYPE> input;
  input.resize(input_size);


  // Old bench
  // const int max_number = 1 << 20;
  // // Will generate numbers between 
  // for (std::size_t i = 0; i < input_size; ++i) { input[i] = rand() % max_number; }

  // Maximum deviation from 1, as we'll use multiplication for reduce
  DATA_TYPE max_deviation = 0.0001;
  DATA_TYPE max = 1. + max_deviation;
  DATA_TYPE min = 1. / max;
  // so that max * min = 1
  // max * min = (1. + max_deviation) * (1. / (1. + max_deviation)) = 1

  srand(9546312);
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = kwk::bench::random_float<DATA_TYPE>(min, max); }

  // std::cout << "Random numbers: ";
  // for (std::size_t i = 0; i < 100; ++i) { std::cout << input[i] << " "; }
  // std::cout << "\n";

  auto view_in  = kwk::view{kwk::source = input.data() , kwk::of_size(d0)};

  DATA_TYPE res_kwk_cpu, res_std, res_hand; // res_std_par

  auto fct_kwk_cpu = [&]() {
    res_kwk_cpu = kwk::reduce(view_in, func, input[0]);
    return res_kwk_cpu; };

  auto fct_std = [&]() {
    res_std = std::reduce(input.begin(), input.end(), input[0], func);
    return res_std; };

  // Huge error message when I try this
  // auto fct_std_par = [&]() {
  //   res_std_par = std::reduce(std::execution::par, input.begin(), input.end(), 0, func);
  //   return res_std_par; };

  auto fct_hand = [&]() {
    res_hand = input[0];
    for (std::size_t i = 1; i < input_size; ++i)
    {
      res_hand = func(res_hand, input[i]);
    }
    return res_hand;
  };

  // TODO: reduce by hand in the most vectorized way possible?
  //       (make the compiler auto-vectorize it by itself as much as possible)
  //       (and/or test with EVE)

  kwk::bench::cbench_t b;
  b.set_iterations(1);

  std::string absolute_path = ""; // will output to "kiwaku_build"

  std::string final_fname = kwk::bench::fprefix() + file_name;

  b.start(absolute_path + final_fname, bench_name, "Processed elements, per second (higher is better)", view_size);
  b.run_function("std::reduce, single thread on CPU", fct_std);
  // b.run_function("std::reduce with std::execution::par", fct_std_par);

  // Don't forget -fsycl-targets=nvptx64-nvidia-cuda
  bool has_gpu = kwk::sycl::has_gpu();

  auto sycl_bench = [&](auto&& context, DATA_TYPE& return_) -> DATA_TYPE
  {
    auto fct_kwk_sycl_generic = [&]()
    {
      return_ = kwk::reduce(context, view_in, func, input[0]);
      return return_;
    };
    b.run_function("Kiwaku SYCL on " + context.get_device_name(), fct_kwk_sycl_generic);
    return return_;
  };

  // Execute SYCL benchmark on GPU and CPU
  if (has_gpu)
  {
    DATA_TYPE return_;
    // GPU
    sycl_bench(kwk::sycl::context{::sycl::gpu_selector_v}, return_);
    TTS_RELATIVE_EQUAL(return_, res_std, ERROR_MAX_PERCENT);
    // CPU
    sycl_bench(kwk::sycl::context{::sycl::cpu_selector_v}, return_);
    TTS_RELATIVE_EQUAL(return_, res_std, ERROR_MAX_PERCENT);
  }
  else // SYCL default context
  {
    DATA_TYPE return_;
    // CPU
    sycl_bench(kwk::sycl::default_context, return_);
    TTS_RELATIVE_EQUAL(return_, res_std, ERROR_MAX_PERCENT);
  }

  b.run_function("Kiwaku on CPU", fct_kwk_cpu);
  b.run_function("By hand on CPU", fct_hand);
  b.stop();

  // TODO: TTS_RELATIVE_EQUAL
  

  // TTS_EQUAL(res_std_par, res_std);
  TTS_RELATIVE_EQUAL(res_hand   , res_std, ERROR_MAX_PERCENT);
  TTS_RELATIVE_EQUAL(res_kwk_cpu, res_std, ERROR_MAX_PERCENT);
}



// TTS_CASE("Benchmark - reduce, compute-bound ")
// {
//   using data_type = int;

//   auto func = [](auto a, auto b) { return (((((((((((a + b) % 4096) % 2048) % 1024) % 512) % 256) % 128) % 64) % 32) % 16) % 8); };

//   [[maybe_unused]] std::size_t kio = 1024 / sizeof(data_type);
//   [[maybe_unused]] std::size_t mio = 1024 * kio;
//   [[maybe_unused]] std::size_t gio = 1024 * mio;
  
//   std::size_t size;
//   std::string hname = sutils::get_host_name();
//        if (hname == "parsys-legend")          { size =   1 * gio; } // 4
//   else if (hname == "pata")                   { size =   1 * gio; }
//   else if (hname == "chaton")                 { size = 128 * mio; }
//   else if (hname == "sylvain-ThinkPad-T580")  { size =  32 * mio; }
//   else                                        { size =   1 * gio; }

//   reduce_test<data_type>("Reduce compute-bound", "reduce_compute-bound.bench", func, size);
// };



TTS_CASE("Benchmark - reduce, memory-bound ")
{

  
  sutils::printer_t::head("Benchmark - Reduce, memory-bound", true);

  using data_type = float;

  auto func = [](data_type a, data_type b) { return a * b; };

  [[maybe_unused]] std::size_t kio = 1024 / sizeof(data_type);
  [[maybe_unused]] std::size_t mio = 1024 * kio;
  [[maybe_unused]] std::size_t gio = 1024 * mio;

  std::size_t size;
  std::string hname = sutils::get_host_name();
       if (hname == "parsys-legend")          { size =   6 * gio; } 
  else if (hname == "pata")                   { size =   1 * gio; }
  else if (hname == "chaton")                 { size = 128 * mio; }
  else if (hname == "sylvain-ThinkPad-T580")  { size =  32 * mio; }
  else if (hname == "sylvain-caillou")        { size =  32 * mio; }
  else                                        { size =   1 * gio; }

  reduce_test<data_type>("Reduce memory-bound", "reduce_memory-bound.bench", func, size);
};
