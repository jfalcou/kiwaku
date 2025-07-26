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
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/reduce.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <numeric>
// For parallel std execution, don't forget the -ltbb compiler flag
// #include <execution>



/// @brief 
/// @tparam DATA_TYPE must be a floating-point type
/// @param bench_name 
/// @param file_name 
/// @param func 
/// @param view_size  
template<typename DATA_TYPE>
void reduce_test( std::string const& bench_name
                , std::string const& file_name
                , auto func
                , auto func_eve
                , DATA_TYPE func_id
                , std::size_t const view_size
                )
{
  std::size_t total_data_size = view_size * sizeof(DATA_TYPE) * 1;

  // The maximum rounding-errors between function results
  const float ERROR_MAX_PERCENT = 50; // 50% difference max

  const std::size_t d0 = view_size;
  const std::size_t input_size = d0;
  std::vector<DATA_TYPE> input;
  input.resize(input_size);

  // Maximum deviation from 1, as we'll use multiplication for reduce
  DATA_TYPE max_deviation = 0.0001;
  DATA_TYPE max = 1. + max_deviation;
  DATA_TYPE min = 1. / max;
  // so that max * min = 1
  // max * min = (1. + max_deviation) * (1. / (1. + max_deviation)) = 1

  srand(9546312);
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = kwk::bench::random_float<DATA_TYPE>(min, max); }

  auto view_in  = kwk::view{kwk::source = input.data() , kwk::of_size(d0)};

  kwk::bench::cbench_t b;
  std::string final_fname = kwk::bench::fprefix() + file_name;
  b.start(final_fname, bench_name, "Throughput", view_size);

  DATA_TYPE res_truth;


  // ====== hand-written sequential ======
  DATA_TYPE res_hand;
  auto fct_hand = [&]()
  {
    res_hand = input[0];
    for (std::size_t i = 1; i < input_size; ++i)
    {
      res_hand = func(res_hand, input[i]);
    }
    return res_hand;
  };
  b.run_function_rpt_bwidth("hand CPU", 1, fct_hand, []{}, total_data_size);
  res_truth = res_hand;


  // ====== Generic Kiwaku bench ======
  auto bench_kiwaku = [&](auto&& context, std::string context_name, auto func_)
  {
    DATA_TYPE ret = -1;
    auto fct_kwk = [&]()
    {
      ret = kwk::reduce(context, view_in, func_, input[0]);
      return ret;
    };
    b.run_function_rpt_bwidth(context_name, 1, fct_kwk, []{}, total_data_size);
    return ret;
  };


  // ====== Kiwaku CPU ======
  DATA_TYPE res_kwk_cpu = bench_kiwaku(::kwk::cpu, "kwk CPU", func);
  TTS_RELATIVE_EQUAL(res_kwk_cpu, res_truth, ERROR_MAX_PERCENT);


  // ====== Kiwaku SIMD ======
  #if KIWAKU_BENCH_EVE
    DATA_TYPE res_kwk_simd;
    auto fct_kwk_eve = [&]()
    {
      res_kwk_simd = kwk::reduce(kwk::simd, view_in, std::pair{func_eve, func_id}, input[0]);
      return res_kwk_simd;
    };
    b.run_function_rpt_bwidth("kwk simd", 1, fct_kwk_eve, []{}, total_data_size);
    TTS_RELATIVE_EQUAL(res_kwk_simd, res_truth, ERROR_MAX_PERCENT);
  #endif


  // ====== Generic std bench ======
  auto bench_std = [&](auto const& policy, std::string context_name)
  {
    DATA_TYPE ret = -1;
    auto fct_std = [&]()
    {
      ret = std::reduce(policy, input.begin(), input.end(), input[0], func);
      return ret;
    };
    b.run_function_rpt_bwidth(context_name, 1, fct_std, []{}, total_data_size);
    return ret;
  };


  // ====== std sequential ======
  DATA_TYPE res_std_seq = bench_std(std::execution::seq, "seq");
  TTS_RELATIVE_EQUAL(res_std_seq, res_truth, ERROR_MAX_PERCENT);


  // ====== std unsequenced ======
  DATA_TYPE res_std_unseq = bench_std(std::execution::unseq, "unseq");
  TTS_RELATIVE_EQUAL(res_std_unseq, res_truth, ERROR_MAX_PERCENT);


  // Don't forget the -ltbb compiler flag
  #if ENABLE_TBB && KIWAKU_BENCH_MTHREAD

    // ====== std parallel ======
    DATA_TYPE res_std_par = bench_std(std::execution::par, "par");
    TTS_RELATIVE_EQUAL(res_std_par, res_truth, ERROR_MAX_PERCENT);

    // ====== std parallel unsequenced ======
    DATA_TYPE res_std_par_unseq = bench_std(std::execution::par_unseq, "par_unseq");
    TTS_RELATIVE_EQUAL(res_std_par_unseq, res_truth, ERROR_MAX_PERCENT);

  #endif // ENABLE_TBB && KIWAKU_BENCH_MTHREAD


  // ====== SYCL ======
  #if KIWAKU_BENCH_SYCL
    // Don't forget -fsycl-targets=nvptx64-nvidia-cuda,x86_64  (with x86_64 or spir64)
    bool has_gpu = kwk::sycl::has_gpu();

    if (has_gpu)
    {
      #if KIWAKU_BENCH_MTHREAD
        // ====== Kiwaku SYCL CPU ======
        auto ctx_cpu = kwk::sycl::context{::sycl::cpu_selector_v};
        int res_sycl_cpu = bench_kiwaku(ctx_cpu, "kwk SYCL " + ctx_cpu.get_device_name(), func);
        TTS_EQUAL(res_sycl_cpu, res_truth);
      #endif

      // ====== Kiwaku SYCL GPU ======
      auto ctx_gpu = kwk::sycl::context{::sycl::gpu_selector_v};
      int res_sycl_gpu = bench_kiwaku(ctx_gpu, "kwk SYCL " + ctx_gpu.get_device_name(), func);
      TTS_EQUAL(res_sycl_gpu, res_truth);
    }
    else // SYCL default context
    {
      #if KIWAKU_BENCH_MTHREAD
        // ====== Kiwaku SYCL CPU ======
        auto& ctx_cpu = kwk::sycl::default_context;
        int res_sycl_cpu = bench_kiwaku(ctx_cpu, "kwk SYCL " + ctx_cpu.get_device_name(), func);
        TTS_EQUAL(res_sycl_cpu, res_truth);
      #endif
    }
  #endif

  b.stop();
}


TTS_CASE("Benchmark - reduce, memory-bound ")
{
  if (::kwk::bench::enable_global)
  {
    ::kwk::bench::get_eve_compiler_flag();

    sutils::printer_t::head("Benchmark - Reduce, memory-bound", true);

    using DATA_TYPE = float;

    auto func = [](auto a, auto b) { return a * b; };

    
    DATA_TYPE func_id = 1; // Neutral element, for addition

    [[maybe_unused]] std::size_t kio = 1024 / sizeof(DATA_TYPE);
    [[maybe_unused]] std::size_t mio = 1024 * kio;
    [[maybe_unused]] std::size_t gio = 1024 * mio;

    std::size_t size;
    std::string hname = sutils::get_host_name();
         if (hname == "parsys-legend")          { size =   6 * gio * kwk::bench::LEGEND_LOAD_FACTOR; } 
    else if (hname == "pata")                   { size =   1 * gio; }
    else if (hname == "chaton")                 { size =   1 * gio; }
    else if (hname == "falcou-avx512")          { size =   6 * gio; }
    else if (hname == "sylvain-ThinkPad-T580")  { size =  32 * mio; }
    else if (hname == "lapierre")               { size =  32 * mio; }
    else                                        { size =   1 * gio; }

    reduce_test<DATA_TYPE>("Reduce memory-bound", "reduce_memory-bound_" + kwk::bench::EVE_COMPILER_FLAG + ".bench", func, func, func_id, size);
  }
  else
  {
    TTS_EQUAL(true, true);
  }
};

#endif