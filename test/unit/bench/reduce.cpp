//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#if KIWAKU_BUILD_BENCH

#include "include/benchmark.hpp"
#include "include/utils/utils.hpp"

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
template<typename DATA_TYPE>
void reduce_test( std::string const& bench_name
                , std::string const& file_name
                , auto func
                , auto func_eve
                , DATA_TYPE func_id
                , std::size_t const L2_length // number of elements contained in L2 cache (number of values, NOT size in bytes)
                , std::size_t const repetitions_over_array
                )
{

  // Always memory-bound

  // Total numer of element processed
  double total_number_of_elements_processed = L2_length * repetitions_over_array;
  double bandwidth_per_element_read   = sizeof(DATA_TYPE);
  double bandwidth_per_element_write  = 0;
  double bandwidth_per_element_in_bytes = bandwidth_per_element_read + bandwidth_per_element_write;

  // The maximum rounding-errors between function results
  const float ERROR_MAX_PERCENT = 500000; // 50% difference max
  constexpr bool check = false;

  std::vector<DATA_TYPE> input(L2_length);

  // Maximum deviation from 1, as we'll use multiplication for reduce
  DATA_TYPE max_deviation = 0.0001;
  DATA_TYPE max = 1. + max_deviation;
  DATA_TYPE min = 1. / max;
  // so that max * min = 1
  // max * min = (1. + max_deviation) * (1. / (1. + max_deviation)) = 1

  srand(9546312);
  for (std::size_t i = 0; i < L2_length; ++i) { input[i] = kwk::bench::random_float<DATA_TYPE>(min, max); }

  auto view_in  = kwk::view{kwk::source = input.data() , kwk::of_size(L2_length)};

  std::string y_axis_title = "Throughput";

  kwk::bench::cbench_t b;
  b.start ( kwk::bench::fprefix() + file_name
          , bench_name
          , "Throughput"
          , total_number_of_elements_processed
          , kwk::bench::bench_type_t::memory
          );

  DATA_TYPE res_truth;


  // ====== hand-written sequential ======
  DATA_TYPE res_hand;
  auto fct_hand = [&]()
  {
    res_hand = 1;
    for (std::size_t r = 0; r < repetitions_over_array; ++r)
    {
      for (std::size_t i = 0; i < L2_length; ++i)
      {
        res_hand = func(res_hand, input[i]);
      }
    }
    return res_hand;
  };
  b.run_ext2("hand CPU"
            , fct_hand
            , []{} // no reset function
            , total_number_of_elements_processed
            , bandwidth_per_element_in_bytes
            , kwk::bench::bench_type_t::memory
            , 1 // osef clock_speed_CPU
            , ::kwk::bench::device_type_t::cpu
            );
  res_truth = res_hand;

  // ====== std::accumulate ======
  DATA_TYPE res_accumulate;
  auto fct_std_accumulate = [&]()
  {
    res_accumulate = 1;
    for (std::size_t r = 0; r < repetitions_over_array; ++r)
      res_accumulate = std::accumulate(input.begin(), input.end(), res_accumulate, std::multiplies<DATA_TYPE>());
    return res_accumulate;
  };
  b.run_ext2("std::accumulate"
            , fct_std_accumulate
            , []{} // no reset function
            , total_number_of_elements_processed
            , bandwidth_per_element_in_bytes
            , kwk::bench::bench_type_t::memory
            , 1 // osef clock_speed_CPU
            , ::kwk::bench::device_type_t::cpu
            );


  // ====== Generic Kiwaku bench ======
  auto bench_kiwaku = [&](auto&& context, std::string context_name, auto func_)
  {
    DATA_TYPE ret;
    auto fct_kwk = [&]()
    {
      ret = 1;
      for (std::size_t r = 0; r < repetitions_over_array; ++r)
        ret = kwk::reduce(context, view_in, func_, input[0]);
      return ret;
    };

    b.run_ext2( context_name
              , fct_kwk
              , []{} // no reset function
              , total_number_of_elements_processed
              , bandwidth_per_element_in_bytes
              , kwk::bench::bench_type_t::memory
              , 1 // osef clock_speed_CPU
              , ::kwk::bench::device_type_t::gpu // osef gpu ou cpu parce que bandwidth
              );
    return ret;
  };

  auto bench_kiwaku_sycl_smart = [&](auto&& context, std::string context_name, auto func_)
  {
    auto in_proxy = context.in(view_in);
    DATA_TYPE ret;
    auto fct_kwk = [&]()
    {
      ret = 1;
      ret = kwk::reduce_proxy(context, view_in, in_proxy, func_, ret, repetitions_over_array);
      return ret;
    };

    b.run_ext2( context_name + " keep proxy alive"
              , fct_kwk
              , []{} // no reset function
              , total_number_of_elements_processed
              , bandwidth_per_element_in_bytes
              , kwk::bench::bench_type_t::memory
              , 1 // osef clock_speed_CPU
              , ::kwk::bench::device_type_t::gpu // osef gpu ou cpu parce que bandwidth
              );
    return ret;
  };


  // ====== Kiwaku CPU ======
  DATA_TYPE res_kwk_cpu = bench_kiwaku(::kwk::cpu, "kwk CPU", func);
  if constexpr(check) TTS_RELATIVE_EQUAL(res_kwk_cpu, res_truth, ERROR_MAX_PERCENT);


  // ====== Kiwaku SIMD ======
  #if KIWAKU_BENCH_EVE
    DATA_TYPE res_kwk_simd = 1;
    auto fct_kwk_eve = [&]()
    {
      for (std::size_t r = 0; r < repetitions_over_array; ++r)
      {
        res_kwk_simd = kwk::reduce(kwk::simd, view_in, std::pair{func_eve, func_id}, res_kwk_simd);
      }
      return res_kwk_simd;
    };

    b.run_ext2( "kwk simd"
              , fct_kwk_eve
              , []{} // no reset function
              , total_number_of_elements_processed
              , bandwidth_per_element_in_bytes
              , kwk::bench::bench_type_t::memory
              , 1 // osef clock_speed_CPU
              , ::kwk::bench::device_type_t::cpu // osef gpu ou cpu parce que bandwidth
              );

    if constexpr(check) TTS_RELATIVE_EQUAL(res_kwk_simd, res_truth, ERROR_MAX_PERCENT);
  #endif


  // ====== Generic std bench ======
  auto bench_std = [&](auto const& policy, std::string context_name)
  {
    DATA_TYPE ret;
    auto fct_std = [&]()
    {
      ret = 1;
      for (std::size_t r = 0; r < repetitions_over_array; ++r)
      {
        ret = std::reduce(policy, input.begin(), input.end(), ret, func);
      }
      return ret;
    };
    b.run_ext2( context_name
              , fct_std
              , []{} // no reset function
              , total_number_of_elements_processed
              , bandwidth_per_element_in_bytes
              , kwk::bench::bench_type_t::memory
              , 1 // osef clock_speed_CPU
              , ::kwk::bench::device_type_t::cpu // osef gpu ou cpu parce que bandwidth
              );
    return ret;
  };


  // ====== std sequential ======
  DATA_TYPE res_std_seq = bench_std(std::execution::seq, "seq");
  if constexpr(check) TTS_RELATIVE_EQUAL(res_std_seq, res_truth, ERROR_MAX_PERCENT);


  // ====== std unsequenced ======
  DATA_TYPE res_std_unseq = bench_std(std::execution::unseq, "unseq");
  if constexpr(check) TTS_RELATIVE_EQUAL(res_std_unseq, res_truth, ERROR_MAX_PERCENT);


  // Don't forget the -ltbb compiler flag
  #if ENABLE_TBB && KIWAKU_BENCH_MTHREAD

    // ====== std parallel ======
    DATA_TYPE res_std_par = bench_std(std::execution::par, "par");
    if constexpr(check) TTS_RELATIVE_EQUAL(res_std_par, res_truth, ERROR_MAX_PERCENT);

    // ====== std parallel unsequenced ======
    DATA_TYPE res_std_par_unseq = bench_std(std::execution::par_unseq, "par_unseq");
    if constexpr(check) TTS_RELATIVE_EQUAL(res_std_par_unseq, res_truth, ERROR_MAX_PERCENT);

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
        if constexpr(check) TTS_RELATIVE_EQUAL(res_sycl_cpu, res_truth, ERROR_MAX_PERCENT);
      #endif

      // ====== Kiwaku SYCL GPU ======
      auto ctx_gpu = kwk::sycl::context{::sycl::gpu_selector_v};
      int res_sycl_gpu = bench_kiwaku(ctx_gpu, "kwk SYCL " + ctx_gpu.get_device_name(), func);
      if constexpr(check) TTS_RELATIVE_EQUAL(res_sycl_gpu, res_truth, ERROR_MAX_PERCENT);

      int res_sycl_gpu2 = bench_kiwaku_sycl_smart(ctx_gpu, "kwk SYCL persistent proxy " + ctx_gpu.get_device_name(), func);
      if constexpr(check) TTS_RELATIVE_EQUAL(res_sycl_gpu2, res_truth, ERROR_MAX_PERCENT);
    }
    else // SYCL default context
    {
      #if KIWAKU_BENCH_MTHREAD
        // ====== Kiwaku SYCL CPU ======
        auto& ctx_cpu = kwk::sycl::default_context;
        int res_sycl_cpu = bench_kiwaku(ctx_cpu, "kwk SYCL " + ctx_cpu.get_device_name(), func);
        if constexpr(check) TTS_RELATIVE_EQUAL(res_sycl_cpu, res_truth, ERROR_MAX_PERCENT);
      #endif
    }
  #endif

  b.stop();
}


TTS_CASE("Benchmark - reduce, L2 memory-bound ")
{
  ::kwk::bench::get_eve_compiler_flag();

  sutils::printer_t::head("Benchmark - Reduce, memory-bound", true);

  using DATA_TYPE = float;

  auto func = [](auto a, auto b) { return a * b; };
  
  DATA_TYPE func_id = 1; // Neutral element, for multiplication

  [[maybe_unused]] std::size_t kio = 1024 / sizeof(DATA_TYPE);
  [[maybe_unused]] std::size_t mio = 1024 * kio;
  [[maybe_unused]] std::size_t gio = 1024 * mio;

  std::size_t size, L2_length;
  std::string hname = sutils::get_host_name();
  if (hname == "parsys-legend")
  {
    size = 6 * gio;
    L2_length = 256 * kio;
  } 
  if (hname == "chaton")
  { 
    size = 1 * gio;
    L2_length = 128 * kio;
  }

  std::size_t repetitions_over_array = size / L2_length;
  std::string l2_str = std::to_string(L2_length / kio);

  reduce_test<DATA_TYPE>( "Reduce L2"
                        , "reduce_L2_" + kwk::bench::EVE_COMPILER_FLAG + "_L2-" + l2_str + ".bench"
                        , func
                        , func // func_eve
                        , func_id
                        , L2_length
                        , repetitions_over_array
                        );
};


TTS_CASE("Benchmark - reduce, RAM memory-bound ")
{
  ::kwk::bench::get_eve_compiler_flag();

  sutils::printer_t::head("Benchmark - Reduce, memory-bound", true);

  using DATA_TYPE = float;

  auto func = [](auto a, auto b) { return a * b; };
  
  DATA_TYPE func_id = 1; // Neutral element, for multiplication

  [[maybe_unused]] std::size_t kio = 1024 / sizeof(DATA_TYPE);
  [[maybe_unused]] std::size_t mio = 1024 * kio;
  [[maybe_unused]] std::size_t gio = 1024 * mio;

  std::size_t size, L2_length;
  std::string hname = sutils::get_host_name();
  if (hname == "parsys-legend")
  {
    size = 6 * gio;
    L2_length = size;
  } 
  if (hname == "chaton")
  { 
    size = 1 * gio;
    L2_length = size;
  }

  std::size_t repetitions_over_array = size / L2_length;
  std::string l2_str = std::to_string(L2_length / kio);

  reduce_test<DATA_TYPE>( "Reduce RAM"
                        , "reduce_RAM_" + kwk::bench::EVE_COMPILER_FLAG + "_L2-" + l2_str + ".bench"
                        , func
                        , func // func_eve
                        , func_id
                        , L2_length
                        , repetitions_over_array
                        );
};

#endif