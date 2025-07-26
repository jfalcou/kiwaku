//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

#if KIWAKU_BUILD_BENCH

#include "../include/benchmark.hpp"
#include "../include/utils/utils.hpp"

#if KIWAKU_BENCH_SYCL
  #include <kwk/context/sycl/context.hpp>
#endif

#include <kwk/context/eve/context.hpp>
#include <kwk/context/sycl/internal/sycl_tools.hpp>
#include <cstdlib>
#include <kwk/algorithm/algos/find.hpp>
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/reduce.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <numeric>
#include <optional>
#include <cmath>
#include <execution> // don't forget the -ltbb compiler flag
#include <iostream>

#include <eve/module/math.hpp>


template<typename DATA_TYPE>
void find_test( std::string const& bench_name
              , std::string const& file_name
              , auto convert_func
              , auto convert_func_eve
              , std::size_t const view_size
              , std::size_t const put_at_pos
              )
{
  // Read-only access
  std::size_t total_data_size = put_at_pos * sizeof(DATA_TYPE) * 1;

  const std::size_t d0 = view_size;
  const std::size_t input_size = d0;
  std::vector<DATA_TYPE> input(input_size);

  DATA_TYPE find_initial_value = -88;
  input[put_at_pos] = find_initial_value;
  DATA_TYPE find_image_value = convert_func(find_initial_value);
  auto compare_func = [=](auto const& item) { return (convert_func(item) == find_image_value); };
  auto compare_func_eve = [&](auto const& item) { return (convert_func_eve(item) == find_image_value); };

  if ( ! compare_func(input[put_at_pos]))
  {
    std::cout << "!=!=!=!=!=!=!=!=!=! ERROR (v1) @find_test: compare_func(input[put_at_pos]) returns false.";
    std::terminate();
  }

  // auto reset_data = [&](std::vector<DATA_TYPE>& vect)
  // {
  //   std::mt19937 gen(78512); // rd()
  //   std::uniform_real_distribution<DATA_TYPE> dis(0, 2.0 * M_PI);
  //   for (std::size_t i = 0; i < L2_size; ++i)
  //     {
  //       vect[i] = dis(gen);
  //     }
  // };

  srand(9546312);
  DATA_TYPE min_value = 0.;
  DATA_TYPE max_value = M_PI * 2;
  // DATA_TYPE max_value = 8465124931.;

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

  if ( ! compare_func(input[put_at_pos]))
  {
    std::cout << "!=!=!=!=!=!=!=!=!=! ERROR (v2) @find_test: compare_func(input[put_at_pos]) returns false.";
    std::terminate();
  }

  auto view_in  = kwk::view{kwk::source = input.data(), kwk::of_size(d0)};

  kwk::bench::cbench_t b;
  std::string absolute_path = ""; // will output to "kiwaku_build"

  b.start(absolute_path + kwk::bench::fprefix() + file_name, bench_name, "Processed elements, per second (higher is better)", view_size);
  // b.set_iterations(1);

  int res_truth;

  // ====== hand-written sequential ======
  int res_hand;
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
  b.run_function("hand CPU", fct_hand);
  res_truth = res_hand;


  // ====== Generic Kiwaku bench ======
  auto bench_kiwaku = [&](auto&& context, std::string context_name, auto compare_func_)
  {
    int ret = -1;
    auto fct_kwk = [&]()
    {
      auto pos = kwk::find_if(context, view_in, compare_func_);
      ret = kwk::utils::tools::pos_to_linear(view_in, pos);
      return ret;
    };
    b.run_function_rpt_bwidth(context_name, 1, fct_kwk, []{}, total_data_size);
    return ret;
  };


  // ====== Kiwaku CPU ======
  int res_kwk_cpu = bench_kiwaku(::kwk::cpu, "kwk CPU", compare_func);
  TTS_EQUAL(res_kwk_cpu, res_truth);


  // ====== Kiwaku SIMD ======
  #if KIWAKU_BENCH_EVE
    int res_kwk_simd = bench_kiwaku(::kwk::simd, "kwk simd", compare_func_eve);
    TTS_EQUAL(res_kwk_simd, res_truth);
  #endif


  // ====== Generic std bench ======
  auto bench_std = [&](auto const& policy, std::string context_name)
  {
    int ret = -1;
    auto fct_std = [&]()
    {
      auto pos = std::find_if(policy, input.begin(), input.end(), compare_func);
      if (pos != input.end()) ret = std::distance(input.begin(), pos);
      else                    ret = -1;
      return ret;
    };
    b.run_function_rpt_bwidth(context_name, 1, fct_std, []{}, total_data_size);
    return ret;
  };


  // ====== std sequential ======
  int res_std_seq = bench_std(std::execution::seq, "seq");
  TTS_EQUAL(res_std_seq, res_truth);


  // ====== std unsequenced ======
  int res_std_unseq = bench_std(std::execution::unseq, "unseq");
  TTS_EQUAL(res_std_unseq, res_truth);


  // Don't forget the -ltbb compiler flag
  #if ENABLE_TBB && KIWAKU_BENCH_MTHREAD

    // ====== std parallel ======
    int res_std_par = bench_std(std::execution::par, "par");
    TTS_EQUAL(res_std_par, res_truth);

    // ====== std parallel unsequenced ======
    int res_std_par_unseq = bench_std(std::execution::par_unseq, "par_unseq");
    TTS_EQUAL(res_std_par_unseq, res_truth);

  #endif // ENABLE_TBB && KIWAKU_BENCH_MTHREAD


  // ====== SYCL ======
  #if KIWAKU_BENCH_SYCL
    // Don't forget -fsycl-targets=nvptx64-nvidia-cuda,x86_64 or spir64
    bool has_gpu = kwk::sycl::has_gpu();

    if (has_gpu)
    {
      // ====== Kiwaku SYCL CPU ======
      auto ctx_cpu = kwk::sycl::context{::sycl::cpu_selector_v};
      int res_sycl_cpu = bench_kiwaku(ctx_cpu, "kwk SYCL " + ctx_cpu.get_device_name(), compare_func);
      TTS_EQUAL(res_sycl_cpu, res_truth);

      // ====== Kiwaku SYCL GPU ======
      auto ctx_gpu = kwk::sycl::context{::sycl::gpu_selector_v};
      int res_sycl_gpu = bench_kiwaku(ctx_gpu, "kwk SYCL " + ctx_gpu.get_device_name(), compare_func);
      TTS_EQUAL(res_sycl_gpu, res_truth);
    }
    else // SYCL default context
    {
      // ====== Kiwaku SYCL CPU ======
      auto& ctx_cpu = kwk::sycl::default_context;
      int res_sycl_cpu = bench_kiwaku(ctx_cpu, "kwk SYCL " + ctx_cpu.get_device_name(), compare_func);
      TTS_EQUAL(res_sycl_cpu, res_truth);
    }
  #endif

  b.stop();
}



enum find_test_pos { first, middle, last };

void find_test_compute_bound(find_test_pos pos)
{
  ::kwk::bench::get_eve_compiler_flag();

  using DATA_TYPE = float;

  // We must ensure type coherency for comparisons to still make sense.
  // auto convert_func = [&](auto const& item)
  // {
    // TODO: test with multiplications and divisions
    // Modulo may imply thread divergence, which would impair GPU performance.
  //   return
  //   static_cast<DATA_TYPE>(
  //     std::cos(
  //       std::sin(
  //         std::cos(
  //           std::sin(
  //             std::cos(
  //               std::sin(
  //                 static_cast<decltype(item)>(item * 11.)
  //                       ) * 3.8
  //                     ) / 1.12
  //                   ) * 3.17874
  //                 ) / 8.7
  //               ) * 9.48
  //             ) / 89.647681
  //           ) * 1.8746221;
  // };

  // auto convert_func_eve = [&](auto const& item)
  // {
  //   return
  //     (::eve::cos(
  //       ::eve::sin(
  //         ::eve::cos(
  //           ::eve::sin(
  //             ::eve::cos(
  //               ::eve::sin(
  //                 item * static_cast<DATA_TYPE>(11.)
  //                       ) * static_cast<DATA_TYPE>(3.8)
  //                     ) / static_cast<DATA_TYPE>(1.12)
  //                   ) * static_cast<DATA_TYPE>(3.17874)
  //                 ) / static_cast<DATA_TYPE>(8.7)
  //               ) * static_cast<DATA_TYPE>(9.48)
  //             ) / static_cast<DATA_TYPE>(89.647681)
  //           ) * static_cast<DATA_TYPE>(1.8746221);
  // };
  // auto convert_func = [&](auto const& item)
  // {
  //   return std::cos( std::sin(item * static_cast<DATA_TYPE>(11.)) * static_cast<DATA_TYPE>(11.)) / static_cast<DATA_TYPE>(89.);
  // };

  auto convert_func_eve = [](auto const& item)
  {
    return
    ::eve::cos(
      ::eve::sin(
        ::eve::cos(
          ::eve::sin(
            ::eve::cos(
              ::eve::sin(
                item * static_cast<DATA_TYPE>(1.425)
              ) * static_cast<DATA_TYPE>(1.458)
            ) / static_cast<DATA_TYPE>(1.578)
          ) * static_cast<DATA_TYPE>(1.98794)
        ) / static_cast<DATA_TYPE>(1.578)
      ) * static_cast<DATA_TYPE>(1.98794)
    ) / static_cast<DATA_TYPE>(1.87961);
    // ::eve::sin(item) * static_cast<DATA_TYPE>(11.)
  };


  auto convert_func = [](auto const& item)
  {
    return
    ::std::cos(
      ::std::sin(
        ::std::cos(
          ::std::sin(
            ::std::cos(
              ::std::sin(
                item * static_cast<DATA_TYPE>(1.425)
              ) * static_cast<DATA_TYPE>(1.458)
            ) / static_cast<DATA_TYPE>(1.578)
          ) * static_cast<DATA_TYPE>(1.98794)
        ) / static_cast<DATA_TYPE>(1.578)
      ) * static_cast<DATA_TYPE>(1.98794)
    ) / static_cast<DATA_TYPE>(1.87961);
    // ::eve::sin(item) * static_cast<DATA_TYPE>(11.)
  };

  [[maybe_unused]] std::size_t kio = 1024 / sizeof(DATA_TYPE);
  [[maybe_unused]] std::size_t mio = 1024 * kio;
  [[maybe_unused]] std::size_t gio = 1024 * mio;

  std::size_t size;
  std::string hname = sutils::get_host_name();
        if (hname == "parsys-legend")          { size =   1 * gio * kwk::bench::LEGEND_LOAD_FACTOR; } 
  else if (hname == "pata")                   { size =  64 * mio; }
  
  else if (hname == "falcou-avx512")          { size = 256 * mio; }
  else if (hname == "chaton")                 { size =  64 * mio; }

  else if (hname == "sylvain-ThinkPad-T580")  { size =  64 * mio; }
  else if (hname == "lapierre")               { size =   8 * mio; }
  else                                        { size =  64 * mio; }
  

  std::string pos_str = "";
  std::size_t put_at_pos = 0;
  switch (pos)
  {
    case first:   pos_str = "pos(first)";  put_at_pos = 0;        break;
    case middle:  pos_str = "pos(middle)"; put_at_pos = size / 2; break;
    case last:    pos_str = "pos(last)";   put_at_pos = size - 1; break;
    default: break;
  }

  sutils::printer_t::head("Benchmark - find-if, compute-bound, " + pos_str, true);

  find_test<DATA_TYPE>("find-if compute-bound " + pos_str
                      , "find-if_compute-bound_" + pos_str + "_" + kwk::bench::EVE_COMPILER_FLAG + "_2PI.bench"
                      , convert_func
                      , convert_func_eve
                      , size
                      , put_at_pos
                      );
  std::cout << "\n\n";
}

// TTS_CASE("Benchmark - find-if, compute-bound, last pos")    { find_test_compute_bound(find_test_pos::last);   };
TTS_CASE("Benchmark - find-if, compute-bound, middle pos")  { find_test_compute_bound(find_test_pos::middle); };
// TTS_CASE("Benchmark - find-if, compute-bound, first pos")   { find_test_compute_bound(find_test_pos::first);  };





void find_test_memory_bound(find_test_pos pos)
{
  ::kwk::bench::get_eve_compiler_flag();

  using DATA_TYPE = float;

  [[maybe_unused]] std::size_t kio = 1024 / sizeof(DATA_TYPE);
  [[maybe_unused]] std::size_t mio = 1024 * kio;
  [[maybe_unused]] std::size_t gio = 1024 * mio;

  std::size_t size;
  std::string hname = sutils::get_host_name();
        if (hname == "parsys-legend")          { size =   6 * gio * kwk::bench::LEGEND_LOAD_FACTOR; } 
  else if (hname == "pata")                   { size = 256 * mio; }

  else if (hname == "falcou-avx512")          { size =   6 * gio; }
  else if (hname == "chaton")                 { size = 256 * mio; }

  else if (hname == "sylvain-ThinkPad-T580")  { size =  32 * mio; }
  else if (hname == "lapierre")               { size =  32 * mio; }
  else                                        { size = 256 * mio; }
  // sutils::printer_t::head("SIZE = " + std::to_string(size), true);

  std::string pos_str = "";
  std::size_t put_at_pos = 0;
  switch (pos)
  {
    case first:   pos_str = "pos(first)";  put_at_pos = 0;        break;
    case middle:  pos_str = "pos(middle)"; put_at_pos = size / 2; break;
    case last:    pos_str = "pos(last)";   put_at_pos = size - 1; break;
    default: break;
  }

  sutils::printer_t::head("Benchmark - find-if, memory-bound, " + pos_str, true);

  auto convert_func = [=](auto const& item) { return item; };
  // auto convert_func = [=](auto const& item) { return ::std::cos(item) * static_cast<DATA_TYPE>(1.45); };
  // auto convert_func_eve = [=](auto const& item) { return ::eve::cos(item) * static_cast<DATA_TYPE>(1.45); };

  find_test<DATA_TYPE>("find-if memory-bound " + pos_str, "find-if-memory-bound_" + pos_str + "_" + kwk::bench::EVE_COMPILER_FLAG + ".bench"
                      , convert_func
                      , convert_func
                      , size
                      , put_at_pos
                      );
  std::cout << "\n\n";
}

// sqrt(4 * cos * cos + sin * sin)

TTS_CASE("Benchmark - find-if, memory-bound, last pos")    { find_test_memory_bound(find_test_pos::last);   };
TTS_CASE("Benchmark - find-if, memory-bound, middle pos")  { find_test_memory_bound(find_test_pos::middle); };
// // TTS_CASE("Benchmark - find-if, memory-bound, first pos")   { find_test_memory_bound(find_test_pos::first);  };


#endif // KIWAKU_BUILD_BENCH