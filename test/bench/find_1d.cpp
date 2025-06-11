//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/context/sycl/context.hpp>
#include <kwk/context/sycl/internal/sycl_tools.hpp>
#include <cstdlib>
#include <kwk/algorithm/algos/find.hpp>
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/reduce.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <numeric>
#include <optional>
#include <benchmark/benchmark.hpp>
#include <benchmark/utils/utils.hpp>
#include <cmath>


// TTS_CASE("Check for kwk::find_if(kwk::sycl::default_context, In, func) 1D")
// {

//   // Array of size 20
//   const std::size_t input_size = 1024 * 1024 * 256 * 4;
//   std::vector<int> input;
//   input.resize(input_size);
//   for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 79865 + 871 ; }
//   auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  
//   int initial_value = 45121;

//   // auto func = [&](auto item) { return (item % initial_value) == 0; };

//   // auto convert_func = [=](auto item)
//   // {
//   //   return
//   //   static_cast<int>(std::cos(std::sin(std::cos(std::sin(static_cast<float>(item * 11) * 7.) * 3.8) * 8.7) * 9.48) * 89647681.) % 1784586545;
//   // };

//   auto convert_func = [=](auto item)
//   {
//     return item;
//   };

//   int image_value = convert_func(initial_value);

//   auto compare_func = [=](auto item)
//   {
//     return (convert_func(item) == image_value);
//   };

//   int put_at_pos = input_size-6;

//   input[put_at_pos] = initial_value;

//   {
//     auto res = kwk::find_if(kwk::sycl::default_context, view, compare_func);
//     if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{put_at_pos});
//     else                 TTS_FAIL("find_if returned std::nullopt and not the expected valid value.");
//   }


//   int res_kwk_sycl, res_kwk_cpu, res_hand, res_std;

//   auto fct_kwk_sycl = [&]()
//   {
//     res_kwk_sycl = kwk::utils::tools::pos_to_linear(view, kwk::find_if(kwk::sycl::default_context, view, compare_func));
//   };

//   auto fct_kwk_cpu = [&]()
//   {
//     res_kwk_cpu = kwk::utils::tools::pos_to_linear(view, kwk::find_if(view, compare_func));
//   };

//   auto fct_std = [&]()
//   {
//     auto res = std::find_if(input.begin(), input.end(), compare_func);
//     if (res != input.end()) res_std = std::distance(input.begin(), res);
//     else                    res_std = -1;
//   };

//   auto fct_hand = [&]()
//   {
//     res_hand = -1;
//     for (std::size_t i = 0; i < input_size; ++i)
//     {
//       if (compare_func(input[i]))
//       {
//         res_hand = i;
//         break;
//       }
//     }
//   };

//   // TODO:   reduce à la main pour voir le temps pris
//   //       + reduce de la manière la plus vectorisée possible ?
//   //         (faire en sorte que le compilateur l'auto-vectorise de lui-même)
//   //         (et/ou tester avec EVE)

//   auto b = kwk::bench::create();
//   kwk::bench::add(b, fct_kwk_cpu , "Kiwaku CPU find_if");
//   kwk::bench::add(b, fct_kwk_sycl, "Kiwaku SYCL find_if");
//   kwk::bench::add(b, fct_std     , "std find_if");
//   kwk::bench::add(b, fct_hand    , "find_if by hand");
//   // b.set_iterations(180);
//   kwk::bench::execute(b);

//   // TTS_RELATIVE_EQUAL(res, res_std, FLOAT_TOLERANCE_PERCENT);
//   TTS_EQUAL(res_kwk_cpu, res_hand);
//   TTS_EQUAL(res_kwk_sycl, res_hand);
//   TTS_EQUAL(res_std     , res_hand);

//   std::cout << "res_hand = " << res_hand << "\n";
//   // TTS_EQUAL(res_hand, res_hand);
// };

template<typename DATA_TYPE>
void find_test( std::string const& bench_name
              , std::string const& file_name
              , auto convert_func
              , std::size_t const view_size
              , std::size_t const put_at_pos
              )
{
  const std::size_t d0 = view_size;
  const std::size_t input_size = d0;
  std::vector<DATA_TYPE> input;
  input.resize(input_size);

  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 79865 + 871 ; }

  auto view_in  = kwk::view{kwk::source = input.data() , kwk::of_size(d0)};

  int initial_value = 45121;
  input[put_at_pos] = initial_value;
  int image_value = convert_func(initial_value);
  auto compare_func = [=](auto item) { return (convert_func(item) == image_value); };

  int res_kwk_cpu, res_std, res_hand; // res_std_par

  // auto fct_kwk_sycl = [&]()
  // {
  //   auto pos = kwk::find_if(kwk::sycl::default_context, view_in, compare_func);
  //   res_kwk_sycl = kwk::utils::tools::pos_to_linear(view_in, pos);
  //   return res_kwk_sycl;
  // };

  auto fct_kwk_cpu = [&]()
  {
    auto pos = kwk::find_if(view_in, compare_func);
    res_kwk_cpu = kwk::utils::tools::pos_to_linear(view_in, pos);
    return res_kwk_cpu;
  };

  auto fct_std = [&]()
  {
    auto pos = std::find_if(input.begin(), input.end(), compare_func);
    if (pos != input.end()) res_std = std::distance(input.begin(), pos);
    else                    res_std = -1;
    return res_std;
  };

  auto fct_hand = [&]()
  {
    res_hand = -1;
    for (std::size_t i = 0; i < input_size; ++i)
    {
      if (compare_func(input[i]))
      {
        res_hand = i;
        break;
      }
    }
  return res_hand;
  };

  kwk::bench::cbench_t b;
  std::string absolute_path = ""; // will output to "kiwaku_build"

  b.start(absolute_path + kwk::bench::fprefix() + file_name, bench_name, "Processed elements, per second (higher is better)", view_size);
  b.set_iterations(10);
  // std::cout << "=============== std::find_if..." << std::flush;
  b.run_function("std::find_if, single thread on CPU", fct_std);
  // std::cout << " OK!\n";

  // Don't forget -fsycl-targets=nvptx64-nvidia-cuda
  bool has_gpu = kwk::sycl::has_gpu();

  auto sycl_bench = [&](auto&& context, DATA_TYPE& return_) -> DATA_TYPE
  {
    auto fct_kwk_sycl_generic = [&]()
    {
      auto pos = kwk::find_if(context, view_in, compare_func);
      return_ = kwk::utils::tools::pos_to_linear(view_in, pos);
      return return_;
    };
    // std::cout << "=============== find_if SYCL..." << std::flush;
    b.run_function("Kiwaku SYCL on " + context.get_device_name(), fct_kwk_sycl_generic);
    // std::cout << " OK!\n";
    return return_;
    // TTS_EQUAL(return_, res_std);
  };

  // Execute SYCL benchmark on GPU and CPU
  if (has_gpu)
  {
    DATA_TYPE return_;
    // GPU
    sycl_bench(kwk::sycl::context{::sycl::gpu_selector_v}, return_);
    TTS_EQUAL(return_, res_std);
    // CPU
    sycl_bench(kwk::sycl::context{::sycl::cpu_selector_v}, return_);
    TTS_EQUAL(return_, res_std);
  }
  else // SYCL default context
  {
    DATA_TYPE return_;
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




TTS_CASE("Benchmark - find-if, compute-bound, last pos")
{
  auto convert_func = [=](auto item)
  {
    // TODO: test with multiplications and divisions
    // Modulo may imply thread divergeance, which would impair GPU performance.
    return
    static_cast<int>(
      std::cos(
        std::sin(
          std::cos(
            std::sin(
              static_cast<float>(item * 11) * 7.
                    ) * 3.8
                  ) * 8.7
                ) * 9.48
              ) * 89647681.
            ) % 1784586545;
  };

  using data_type = int;
  [[maybe_unused]] std::size_t kio = 1024 / sizeof(data_type);
  [[maybe_unused]] std::size_t mio = 1024 * kio;
  [[maybe_unused]] std::size_t gio = 1024 * mio;

  std::size_t size;
  std::string hname = sutils::get_host_name();
       if (hname == "parsys-legend")          { size = 256 * mio; } 
  else if (hname == "pata")                   { size =  64 * mio; }
  else if (hname == "chaton")                 { size =  64 * mio; }
  else if (hname == "sylvain-ThinkPad-T580")  { size =   8 * mio; }
  else                                        { size =  64 * mio; }

  // const std::size_t d0 = 1024 * 1024 * 16;
  
  find_test<data_type>("find-if compute-bound", "find-if_compute-bound.txt", convert_func, size, size-2);
  std::cout << "\n\n";
};


TTS_CASE("Benchmark - find-if, memory-bound, last pos")
{
  using data_type = int;
  [[maybe_unused]] std::size_t kio = 1024 / sizeof(data_type);
  [[maybe_unused]] std::size_t mio = 1024 * kio;
  [[maybe_unused]] std::size_t gio = 1024 * mio;

  std::size_t size;
  std::string hname = sutils::get_host_name();
       if (hname == "parsys-legend")          { size =  10 * gio; } 
  else if (hname == "pata")                   { size = 256 * mio; }
  else if (hname == "chaton")                 { size = 256 * mio; }
  else if (hname == "sylvain-ThinkPad-T580")  { size =  32 * mio; }
  else                                        { size = 256 * mio; }

  auto convert_func = [=](auto item) { return item; };

  find_test<data_type>("find-if memory-bound", "find-if_memory-bound.txt", convert_func, size, size-2);
  std::cout << "\n\n";
};


TTS_CASE("Benchmark - find-if, compute-bound, middle")
{
  using data_type = int;
  [[maybe_unused]] std::size_t kio = 1024 / sizeof(data_type);
  [[maybe_unused]] std::size_t mio = 1024 * kio;
  [[maybe_unused]] std::size_t gio = 1024 * mio;

  std::size_t size;
  std::string hname = sutils::get_host_name();
       if (hname == "parsys-legend")          { size = 256 * mio; } 
  else if (hname == "pata")                   { size =  64 * mio; }
  else if (hname == "chaton")                 { size =  64 * mio; }
  else if (hname == "sylvain-ThinkPad-T580")  { size =   8 * mio; }
  else                                        { size =  64 * mio; }

  auto convert_func = [=](auto item)
  {
    // TODO: test with multiplications and divisions
    // Modulo may imply thread divergeance, which would impair GPU performance.
    return
    static_cast<int>(
      std::cos(
        std::sin(
          std::cos(
            std::sin(
              static_cast<float>(item * 11) * 7.
                    ) * 3.8
                  ) * 8.7
                ) * 9.48
              ) * 89647681.
            ) % 1784586545;
  };

  find_test<data_type>("find-if compute-bound", "find-if_compute-bound_middle.txt", convert_func, size, size / 2);
  std::cout << "\n\n";
};


TTS_CASE("Benchmark - find-if, memory-bound, middle")
{
  using data_type = int;
  [[maybe_unused]] std::size_t kio = 1024 / sizeof(data_type);
  [[maybe_unused]] std::size_t mio = 1024 * kio;
  [[maybe_unused]] std::size_t gio = 1024 * mio;

  std::size_t size;
  std::string hname = sutils::get_host_name();
       if (hname == "parsys-legend")          { size =  10 * gio; } 
  else if (hname == "pata")                   { size = 256 * mio; }
  else if (hname == "chaton")                 { size = 256 * mio; }
  else if (hname == "sylvain-ThinkPad-T580")  { size =  32 * mio; }
  else                                        { size = 256 * mio; }

  auto convert_func = [=](auto item) { return item; };

  find_test<data_type>("find-if memory-bound", "find-if_memory-bound_middle.txt", convert_func, size, size / 2);
  std::cout << "\n\n";
};
