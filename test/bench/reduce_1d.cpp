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
#include <benchmark/benchmark.hpp>
// #include <execution>

// TTS_CASE("Benchmark - reduce ")
// {
//   using data_type = int;
//   const std::size_t d0 = 1024 * 1024 * 256; // 256
//   const std::size_t input_size = d0;
//   std::vector<data_type> input;
//   input.resize(input_size);

//   for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 3; }

//   auto view_in  = kwk::view{kwk::source = input.data() , kwk::of_size(d0)};

//   data_type res_kwk_cpu, res_std, res_hand; // res_std_par

//   // auto func = [](auto a, auto b) { return (((((((((a + b) % 60) % 59) % 58) % 57) % 56) % 55) % 35) % 25) % 15; };
//   auto func = [](auto a, auto b) { return (((((((((((a + b) % 4096) % 2048) % 1024) % 512) % 256) % 128) % 64) % 32) % 16) % 8); };

  

//   auto fct_kwk_cpu = [&]() {
//     res_kwk_cpu = kwk::reduce(view_in, func, 0);
//     return res_kwk_cpu; };

//   auto fct_std = [&]() {
//     res_std = std::reduce(input.begin(), input.end(), 0, func);
//     return res_std; };

//   // Huge error message when I try this
//   // auto fct_std_par = [&]() {
//   //   res_std_par = std::reduce(std::execution::par, input.begin(), input.end(), 0, func);
//   //   return res_std_par; };

//   auto fct_hand = [&]() {
//     res_hand = 0;
//     for (std::size_t i = 0; i < input_size; ++i)
//     {
//       res_hand = func(res_hand, input[i]);
//     }
//     return res_hand;
//   };

//   // TODO:   reduce à la main pour voir le temps pris
//   //       + reduce de la manière la plus vectorisée possible ?
//   //         (faire en sorte que le compilateur l'auto-vectorise de lui-même)
//   //         (et/ou tester avec EVE)

//   kwk::bench::cbench_t b;

//   // std::string absolute_path = "/home/data_evo/data_sync/academique/These/kiwaku_2025-02/test/bench/bench_files/";
//   std::string absolute_path = ""; // will output so "kiwaku_build"

//   b.start(absolute_path + "reduce_1d_2025-06-04_19h15.txt", "Reduce, compute-bound");
//   b.set_iterations(10);
//   b.run_function("std::reduce", fct_std);
//   // b.run_function("std::reduce with std::execution::par", fct_std_par);

//   // Don't forget -fsycl-targets=nvptx64-nvidia-cuda
//   bool has_gpu = kwk::sycl::has_gpu();

//   auto sycl_bench = [&](auto&& context, data_type& return_) -> data_type
//   {
//     auto fct_kwk_sycl_generic = [&]()
//     {
//       return_ = kwk::reduce(context, view_in, func, 0);
//       return return_;
//     };
//     b.run_function("Kiwaku SYCL on " + context.get_device_name(), fct_kwk_sycl_generic);
//     return return_;
//     // TTS_EQUAL(return_, res_std);
//   };

//   // Execute SYCL benchmark on GPU and CPU
//   if (has_gpu)
//   {
//     data_type return_;
//     // GPU
//     sycl_bench(kwk::sycl::context{::sycl::gpu_selector_v}, return_);
//     TTS_EQUAL(return_, res_std);
//     // CPU
//     sycl_bench(kwk::sycl::context{::sycl::cpu_selector_v}, return_);
//     TTS_EQUAL(return_, res_std);

//     // // Run on GPU
//     // data_type res_kwk_sycl_gpu;
//     // kwk::sycl::context gpu_context{::sycl::gpu_selector_v};

//     // auto fct_kwk_sycl_gpu = [&res_kwk_sycl, &view_in, func]() {
//     //   res_kwk_sycl_gpu = kwk::reduce(gpu_context, view_in, func, 0);
//     //   return res_kwk_sycl_gpu; };
//     // TTS_EQUAL(res_kwk_sycl_gpu, res_std);
//     // b.run_function("Kiwaku SYCL GPU " + gpu_context.get_device_name(), fct_kwk_sycl_gpu);

//     // // Run on CPU
//     // data_type res_kwk_sycl_cpu;
//     // kwk::sycl::context cpu_context{::sycl::cpu_selector_v};

//     // auto fct_kwk_sycl_cpu = [&res_kwk_sycl, &view_in, func]() {
//     //   res_kwk_sycl_cpu = kwk::reduce(cpu_context, view_in, func, 0);
//     //   return res_kwk_sycl_cpu; };
//     // TTS_EQUAL(res_kwk_sycl_cpu, res_std);
//     // b.run_function("Kiwaku SYCL CPU " + cpu_context.get_device_name(), fct_kwk_sycl_cpu);
//   }
//   else // SYCL default context
//   {
//     data_type return_;
//     // CPU
//     sycl_bench(kwk::sycl::default_context, return_);
//     TTS_EQUAL(return_, res_std);
//   }

//   b.run_function("Kiwaku on CPU", fct_kwk_cpu);
//   b.run_function("By hand on CPU", fct_hand);
//   b.stop();

//   // TTS_EQUAL(res_std_par, res_std);
//   TTS_EQUAL(res_hand   , res_std);
//   TTS_EQUAL(res_kwk_cpu, res_std);
// };


void reduce_test(std::string const& bench_name, std::string const& file_name, auto func, std::size_t const view_size)
{
  using data_type = int;
  const std::size_t d0 = view_size;//1024 * 1024 * 256; // 256
  const std::size_t input_size = d0;
  std::vector<data_type> input;
  input.resize(input_size);

  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 3; }

  auto view_in  = kwk::view{kwk::source = input.data() , kwk::of_size(d0)};

  data_type res_kwk_cpu, res_std, res_hand; // res_std_par

  // auto func = [](auto a, auto b) { return (((((((((a + b) % 60) % 59) % 58) % 57) % 56) % 55) % 35) % 25) % 15; };
  // auto func = [](auto a, auto b) { return (((((((((((a + b) % 4096) % 2048) % 1024) % 512) % 256) % 128) % 64) % 32) % 16) % 8); };

  

  auto fct_kwk_cpu = [&]() {
    res_kwk_cpu = kwk::reduce(view_in, func, 0);
    return res_kwk_cpu; };

  auto fct_std = [&]() {
    res_std = std::reduce(input.begin(), input.end(), 0, func);
    return res_std; };

  // Huge error message when I try this
  // auto fct_std_par = [&]() {
  //   res_std_par = std::reduce(std::execution::par, input.begin(), input.end(), 0, func);
  //   return res_std_par; };

  auto fct_hand = [&]() {
    res_hand = 0;
    for (std::size_t i = 0; i < input_size; ++i)
    {
      res_hand = func(res_hand, input[i]);
    }
    return res_hand;
  };

  // TODO:   reduce à la main pour voir le temps pris
  //       + reduce de la manière la plus vectorisée possible ?
  //         (faire en sorte que le compilateur l'auto-vectorise de lui-même)
  //         (et/ou tester avec EVE)

  kwk::bench::cbench_t b;

  // std::string absolute_path = "/home/data_evo/data_sync/academique/These/kiwaku_2025-02/test/bench/bench_files/";
  std::string absolute_path = ""; // will output so "kiwaku_build"

  b.start(absolute_path + file_name, bench_name);
  b.set_iterations(10);
  b.run_function("std::reduce", fct_std);
  // b.run_function("std::reduce with std::execution::par", fct_std_par);

  // Don't forget -fsycl-targets=nvptx64-nvidia-cuda
  bool has_gpu = kwk::sycl::has_gpu();

  auto sycl_bench = [&](auto&& context, data_type& return_) -> data_type
  {
    auto fct_kwk_sycl_generic = [&]()
    {
      return_ = kwk::reduce(context, view_in, func, 0);
      return return_;
    };
    b.run_function("Kiwaku SYCL on " + context.get_device_name(), fct_kwk_sycl_generic);
    return return_;
    // TTS_EQUAL(return_, res_std);
  };

  // Execute SYCL benchmark on GPU and CPU
  if (has_gpu)
  {
    data_type return_;
    // GPU
    sycl_bench(kwk::sycl::context{::sycl::gpu_selector_v}, return_);
    TTS_EQUAL(return_, res_std);
    // CPU
    sycl_bench(kwk::sycl::context{::sycl::cpu_selector_v}, return_);
    TTS_EQUAL(return_, res_std);

    // // Run on GPU
    // data_type res_kwk_sycl_gpu;
    // kwk::sycl::context gpu_context{::sycl::gpu_selector_v};

    // auto fct_kwk_sycl_gpu = [&res_kwk_sycl, &view_in, func]() {
    //   res_kwk_sycl_gpu = kwk::reduce(gpu_context, view_in, func, 0);
    //   return res_kwk_sycl_gpu; };
    // TTS_EQUAL(res_kwk_sycl_gpu, res_std);
    // b.run_function("Kiwaku SYCL GPU " + gpu_context.get_device_name(), fct_kwk_sycl_gpu);

    // // Run on CPU
    // data_type res_kwk_sycl_cpu;
    // kwk::sycl::context cpu_context{::sycl::cpu_selector_v};

    // auto fct_kwk_sycl_cpu = [&res_kwk_sycl, &view_in, func]() {
    //   res_kwk_sycl_cpu = kwk::reduce(cpu_context, view_in, func, 0);
    //   return res_kwk_sycl_cpu; };
    // TTS_EQUAL(res_kwk_sycl_cpu, res_std);
    // b.run_function("Kiwaku SYCL CPU " + cpu_context.get_device_name(), fct_kwk_sycl_cpu);
  }
  else // SYCL default context
  {
    data_type return_;
    // CPU
    sycl_bench(kwk::sycl::default_context, return_);
    TTS_EQUAL(return_, res_std);
  }

  b.run_function("Kiwaku on CPU", fct_kwk_cpu);
  b.run_function("By hand on CPU", fct_hand);
  b.stop();

  // TTS_EQUAL(res_std_par, res_std);
  TTS_EQUAL(res_hand   , res_std);
  TTS_EQUAL(res_kwk_cpu, res_std);
}


// TTS_CASE("Benchmark - reduce, compute-bound ")
// {
//   auto func = [](auto a, auto b) { return (((((((((((a + b) % 4096) % 2048) % 1024) % 512) % 256) % 128) % 64) % 32) % 16) % 8); };
//   const std::size_t d0 = 1024 * 1024 * 256; // 256

//   reduce_test("Reduce compute-bound", "reduce_compute-bound_2025-06-04_23h11.txt", func, d0);
// };




TTS_CASE("Benchmark - reduce, memory-bound ")
{
  auto func = [](auto a, auto b) { return a + b; };
  const std::size_t d0 = 1024 * 1024 * 400; // 256

  reduce_test("Reduce memory-bound", "reduce_memory-bound_2025-06-04_23h11.txt", func, d0);
};
