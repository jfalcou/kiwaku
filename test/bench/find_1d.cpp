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
#include "include/benchmark.hpp"
#include "include/utils/utils.hpp"
#include <cmath>


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

  DATA_TYPE find_initial_value = -88;
  input[put_at_pos] = find_initial_value;
  DATA_TYPE find_image_value = convert_func(find_initial_value);
  auto compare_func = [=](auto item) { return (convert_func(item) == find_image_value); };
  std::cout << "START - input[put_at_pos] = " << input[put_at_pos] << " find_image_value = " << find_image_value << "\n";

  srand(9546312);
  DATA_TYPE min_value = 0.;
  DATA_TYPE max_value = 8465124931.;

  for (std::size_t i = 0; i < input_size; ++i)
  {
    // Ignore the find position
    if (i == put_at_pos) continue;

    // We want a random value that has an image by convert_func
    // that does not match find_image_value.
    // So we keep generating a random value until the value is valid. 
    std::size_t rpt = 0;
    do
    {
      input[i] = kwk::bench::random_float<DATA_TYPE>(min_value, max_value);
      ++rpt;
      if (rpt > 1) { std::cout << "!! find_test: !! Erroneous value, same image, rpt = " << rpt << ", pos = " << i << "\n"; }
    }
    while (convert_func(input[i]) == find_image_value);
  }

  std::cout << "AFTER FILL - input[put_at_pos] = " << input[put_at_pos] << " find_image_value = " << find_image_value << "\n";
  std::cout << "AFTER FILL - compare_func(input[put_at_pos]) = " << compare_func(input[put_at_pos]) << "\n";
  std::cout << "AFTER FILL - compare_func(input[put_at_pos - 1]) = " << compare_func(input[put_at_pos - 1]) << "\n";

  auto view_in  = kwk::view{kwk::source = input.data(), kwk::of_size(d0)};

  int res_kwk_cpu, res_std, res_hand; // res_std_par

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
  b.set_iterations(1);
  b.run_function("std::find_if, single thread on CPU", fct_std);

  // Don't forget -fsycl-targets=nvptx64-nvidia-cuda
  bool has_gpu = kwk::sycl::has_gpu();

  auto sycl_bench = [&](auto&& context, int& return_) -> int
  {
    auto fct_kwk_sycl_generic = [&]()
    {
      auto pos = kwk::find_if(context, view_in, compare_func);
      return_ = kwk::utils::tools::pos_to_linear(view_in, pos);
      return return_;
    };
    b.run_function("Kiwaku SYCL on " + context.get_device_name(), fct_kwk_sycl_generic);
    return return_;
  };

  // Execute SYCL benchmark on GPU and CPU
  if (has_gpu)
  {
    int return_;
    // GPU
    sycl_bench(kwk::sycl::context{::sycl::gpu_selector_v}, return_);
    TTS_EQUAL(return_, res_std);
    // CPU
    sycl_bench(kwk::sycl::context{::sycl::cpu_selector_v}, return_);
    TTS_EQUAL(return_, res_std);
  }
  else // SYCL default context
  {
    int return_;
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

  sutils::printer_t::head("Benchmark - find-if, compute-bound, last pos", true);

  using data_type = float;
  // We must ensure type coherency for comparisons to still make sense.
  auto convert_func = [=](data_type item) -> data_type
  {
    // TODO: test with multiplications and divisions
    // Modulo may imply thread divergence, which would impair GPU performance.
    return
    static_cast<data_type>(
      std::cos(
        std::sin(
          std::cos(
            std::sin(
              std::cos(
                std::sin(
                  static_cast<float>(item * 11.) * 7.
                        ) * 3.8
                      ) / 1.12
                    ) * 3.17874
                  ) / 8.7
                ) * 9.48
              ) / 89.647681
            ) * 1.8746221;
  };

  [[maybe_unused]] std::size_t kio = 1024 / sizeof(data_type);
  [[maybe_unused]] std::size_t mio = 1024 * kio;
  [[maybe_unused]] std::size_t gio = 1024 * mio;

  std::size_t size;
  std::string hname = sutils::get_host_name();
       if (hname == "parsys-legend")          { size =   1 * gio; } 
  else if (hname == "pata")                   { size =  64 * mio; }
  else if (hname == "chaton")                 { size =  64 * mio; }
  else if (hname == "sylvain-ThinkPad-T580")  { size =   8 * mio; }
  else                                        { size =  64 * mio; }

  // const std::size_t d0 = 1024 * 1024 * 16;
  
  find_test<data_type>("find-if compute-bound", "find-if_compute-bound.bench", convert_func, size, size-2);
  std::cout << "\n\n";
};

// sqrt(4 * cos * cos + sin * sin)

TTS_CASE("Benchmark - find-if, memory-bound, last pos")
{
  sutils::printer_t::head("Benchmark - find-if, memory-bound, last pos", true);

  using data_type = float;
  [[maybe_unused]] std::size_t kio = 1024 / sizeof(data_type);
  [[maybe_unused]] std::size_t mio = 1024 * kio;
  [[maybe_unused]] std::size_t gio = 1024 * mio;

  std::size_t size;
  std::string hname = sutils::get_host_name();
       if (hname == "parsys-legend")          { size =   6 * gio; } 
  else if (hname == "pata")                   { size = 256 * mio; }
  else if (hname == "chaton")                 { size = 256 * mio; }
  else if (hname == "sylvain-ThinkPad-T580")  { size =  32 * mio; }
  else                                        { size = 256 * mio; }

  auto convert_func = [=](data_type item) -> data_type { return item; };

  find_test<data_type>("find-if memory-bound", "find-if_memory-bound.bench", convert_func, size, size-2);
  std::cout << "\n\n";
};


// ========== MIDDLE ==========

// TTS_CASE("Benchmark - find-if, compute-bound, middle")
// {
//   using data_type = float;
//   [[maybe_unused]] std::size_t kio = 1024 / sizeof(data_type);
//   [[maybe_unused]] std::size_t mio = 1024 * kio;
//   [[maybe_unused]] std::size_t gio = 1024 * mio;

//   std::size_t size;
//   std::string hname = sutils::get_host_name();
//        if (hname == "parsys-legend")          { size = 256 * mio; } 
//   else if (hname == "pata")                   { size =  64 * mio; }
//   else if (hname == "chaton")                 { size =  64 * mio; }
//   else if (hname == "sylvain-ThinkPad-T580")  { size =   8 * mio; }
//   else                                        { size =   8 * mio; }

//   auto convert_func = [=](auto item)
//   {
//     // TODO: test with multiplications and divisions
//     // Modulo may imply thread divergence, which would impair GPU performance.
//     return
//     static_cast<data_type>(
//       std::cos(
//         std::sin(
//           std::cos(
//             std::sin(
//               std::cos(
//                 std::sin(
//                   static_cast<float>(item * 11.) * 7.
//                         ) * 3.8
//                       ) / 1.12
//                     ) * 3.17874
//                   ) / 8.7
//                 ) * 9.48
//               ) / 89.647681
//             ) * 1.8746221;
//   };

//   find_test<data_type>("find-if compute-bound", "find-if_compute-bound_middle.bench", convert_func, size, size / 2);
//   std::cout << "\n\n";
// };



// TTS_CASE("Benchmark - find-if, memory-bound, middle")
// {
//   using data_type = float;
//   [[maybe_unused]] std::size_t kio = 1024 / sizeof(data_type);
//   [[maybe_unused]] std::size_t mio = 1024 * kio;
//   [[maybe_unused]] std::size_t gio = 1024 * mio;

//   std::size_t size;
//   std::string hname = sutils::get_host_name();
//        if (hname == "parsys-legend")          { size =  10 * gio; }
//   else if (hname == "pata")                   { size = 256 * mio; }
//   else if (hname == "chaton")                 { size = 256 * mio; }
//   else if (hname == "sylvain-ThinkPad-T580")  { size =  32 * mio; }
//   else                                        { size =  32 * mio; }

//   auto convert_func = [=](auto item) { return item; };

//   find_test<data_type>("find-if memory-bound", "find-if_memory-bound_middle.bench", convert_func, size, size / 2);
//   std::cout << "\n\n";
// };
