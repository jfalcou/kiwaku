//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

#if KIWAKU_BUILD_BENCH

#include <cstdlib>
#include <kwk/context/eve/context.hpp>
#include <kwk/context/sycl/internal/sycl_tools.hpp>
#include <kwk/algorithm/algos/numeric.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <numeric>
#include "../include/benchmark.hpp"
#include "../include/utils/utils.hpp"
#include <cmath>
#include <execution> // don't forget the -ltbb compiler flag

#include <eve/module/math.hpp>


template<typename DATA_TYPE>
void transform_reduce_test(std::string const& bench_name
                          , std::string const& file_name
                          , auto reduce_func
                          , auto reduce_func_id // neutral element for reduce
                          , auto transform_func
                          , auto transform_func_eve
                          , std::size_t const view_size
                          )
{
  std::size_t total_data_size = view_size * sizeof(DATA_TYPE) * 2;

  const std::size_t input_size = view_size;
  std::vector<DATA_TYPE> input1;
  std::vector<DATA_TYPE> input2;
  input1.resize(input_size);
  input2.resize(input_size);

  DATA_TYPE init_value = 11;

  std::mt19937 gen(78512);
  std::uniform_real_distribution<DATA_TYPE> dis(0, 2.0 * M_PI);
  for (std::size_t i = 0; i < input_size; ++i)
  {
    input1[i] = dis(gen);
    input2[i] = dis(gen);
  }

  auto view1 = kwk::view{kwk::source = input1, kwk::of_size(input_size)};
  auto view2 = kwk::view{kwk::source = input2, kwk::of_size(input_size)};


  std::string absolute_path = ""; // will output to "kiwaku_build"
  std::string final_fname = kwk::bench::fprefix() + file_name;

  kwk::bench::cbench_t b;
  b.start(absolute_path + final_fname, bench_name, "Processed elements, per second (higher is better)", view_size);


  // ====== Generic Kiwaku bench ======
  // Only for kwk::cpu and kwk::sycl
  // !! Not compatible with kwk::simd !!
  auto bench_kiwaku = [&](auto&& context, std::string context_name)
  {
    DATA_TYPE ret = -1;
    auto fct_kwk = [&]()
    {
      ret = kwk::transform_reduce(context, view1, view2, init_value, reduce_func, transform_func);
      return ret;
    };
    b.run_function_rpt_bwidth(context_name, 1, fct_kwk, []{}, total_data_size);
    return ret;
  };


  DATA_TYPE res_truth;


  // ====== hand-written sequential ======
  DATA_TYPE res_hand;
  auto fct_hand = [&]()
  {
    res_hand = init_value;
    for (std::size_t i = 0; i < input_size; ++i)
    {
      res_hand = reduce_func(res_hand, transform_func(input1[i], input2[i]));
    }
    return res_hand;
  };
  b.run_function_rpt_bwidth("hand CPU", 1, fct_hand, []{}, total_data_size);
  res_truth = res_hand;


  // ====== Kiwaku CPU ======
  DATA_TYPE res_kwk_cpu = bench_kiwaku(::kwk::cpu, "kwk CPU");
  TTS_RELATIVE_EQUAL(res_kwk_cpu, res_truth, FLOAT_TOLERANCE_PERCENT_BENCH);


  // ====== Kiwaku SIMD ======
  #if KIWAKU_BENCH_EVE
  DATA_TYPE res_kwk_simd;
    auto fct_kwk_simd = [&]()
    {
      res_kwk_simd = kwk::transform_reduce( ::kwk::simd
                                          , view1
                                          , view2
                                          , init_value
                                          , std::pair{reduce_func, reduce_func_id}
                                          , transform_func_eve
                                          );
      return res_kwk_simd;
    };
    b.run_function_rpt_bwidth("kwk simd", 1, fct_kwk_simd, []{}, total_data_size);
    TTS_RELATIVE_EQUAL(res_kwk_simd, res_truth, FLOAT_TOLERANCE_PERCENT_BENCH);
  #endif


  // ====== Generic std bench ======
  auto bench_std = [&](auto const& policy, std::string context_name)
  {
    DATA_TYPE ret = -1;
    auto fct_std = [&]()
    {
      ret = std::transform_reduce ( policy
                                  , input1.begin()
                                  , input1.end()
                                  , input2.begin()
                                  , init_value
                                  , reduce_func
                                  , transform_func
                                  );
      return ret;
    };
    b.run_function_rpt_bwidth(context_name, 1, fct_std, []{}, total_data_size);
    return ret;
  };


  // ====== std sequential ======
  DATA_TYPE res_std_seq = bench_std(std::execution::seq, "seq");
  TTS_RELATIVE_EQUAL(res_std_seq, res_truth, FLOAT_TOLERANCE_PERCENT_BENCH);


  // ====== std unsequenced ======
  DATA_TYPE res_std_unseq = bench_std(std::execution::unseq, "unseq");
  TTS_RELATIVE_EQUAL(res_std_unseq, res_truth, FLOAT_TOLERANCE_PERCENT_BENCH);


  // Don't forget the -ltbb compiler flag
  #if ENABLE_TBB && KIWAKU_BENCH_MTHREAD

    // ====== std parallel ======
    DATA_TYPE res_std_par = bench_std(std::execution::par, "par");
    TTS_RELATIVE_EQUAL(res_std_par, res_truth, FLOAT_TOLERANCE_PERCENT_BENCH);

    // ====== std parallel unsequenced ======
    DATA_TYPE res_std_par_unseq = bench_std(std::execution::par_unseq, "par_unseq");
    TTS_RELATIVE_EQUAL(res_std_par_unseq, res_truth, FLOAT_TOLERANCE_PERCENT_BENCH);

  #endif // ENABLE_TBB && KIWAKU_BENCH_MTHREAD

  b.stop();
}



TTS_CASE("Benchmark - transform_reduce, compute-bound ")
{
  ::kwk::bench::get_eve_compiler_flag();

  using DATA_TYPE = float;

  auto reduce_func    = [](auto a, auto b) { return a * b; };
  auto transform_func = [](auto a, auto b)
  {
    // cos²(a) + sin²(a) = 1
    // cos²(b) + sin²(b) = 1
    return  (
              std::cos(a) * 1.86413 * std::cos(a) 
            + std::cos(b) * 1.86413 * std::cos(b)
            + std::sin(a) * 1.86413 * std::sin(a) 
            + std::sin(b) * 1.86413 * std::sin(b)
            ) / (1.86413 * 2);
  };

  auto transform_func_eve = [](auto a, auto b)
  {
    // cos²(a) + sin²(a) = 1
    // cos²(b) + sin²(b) = 1
    return  (
              eve::cos(a) * 1.86413 * eve::cos(a) 
            + eve::cos(b) * 1.86413 * eve::cos(b)
            + eve::sin(a) * 1.86413 * eve::sin(a) 
            + eve::sin(b) * 1.86413 * eve::sin(b)
            ) / (1.86413 * 2);
  };
  // auto reduce_func    = [](auto a, auto b) { return (((((((((((a + b) % 4096) % 2048) % 1024) % 512) % 256) % 128) % 64) % 32) % 16) % 8); };
  // auto transform_func = [](auto a, auto b) { return ((((((((((((a + b) % 131072) % 65536) % 32768) % 16384) % 8192) % 4096) % 2048) % 1024) % 512) % 256) % 128); };

  [[maybe_unused]] std::size_t kio = 1024 / (sizeof(DATA_TYPE) * 2);
  [[maybe_unused]] std::size_t mio = 1024 * kio;
  [[maybe_unused]] std::size_t gio = 1024 * mio;

  std::size_t size;
  std::string hname = sutils::get_host_name();
        if (hname == "parsys-legend")          { size =   1 * gio * kwk::bench::LEGEND_LOAD_FACTOR; } 
  else if (hname == "falcou-avx512")          { size =   1 * gio; }
  else if (hname == "pata")                   { size =   1 * gio; }
  else if (hname == "chaton")                 { size = 128 * mio; }
  else if (hname == "sylvain-ThinkPad-T580")  { size =  32 * mio; }
  else if (hname == "lapierre")               { size =  32 * mio; }
  else                                        { size =   1 * gio; }

  sutils::printer_t::head("Benchmark - transform_reduce, compute-bound", true);

  transform_reduce_test<DATA_TYPE>("Transform_reduce compute-bound"
    , "transform_reduce_compute-bound_" + kwk::bench::EVE_COMPILER_FLAG + ".bench"
    , reduce_func
    , 1 // neutral element for the reduce function
    , transform_func
    , transform_func_eve
    , size
    );
};



TTS_CASE("Benchmark - transform_reduce, memory-bound ")
{
  ::kwk::bench::get_eve_compiler_flag();
  
  using DATA_TYPE = float;

  auto reduce_func    = [](auto a, auto b) { return (a + b); };
  auto transform_func = [](auto a, auto b) { return (a * b); };

  [[maybe_unused]] std::size_t kio = 1024 / (sizeof(DATA_TYPE) + sizeof(DATA_TYPE));
  [[maybe_unused]] std::size_t mio = 1024 * kio;
  [[maybe_unused]] std::size_t gio = 1024 * mio;

  std::size_t size;
  std::string hname = sutils::get_host_name();
        if (hname == "parsys-legend")          { size =   6 * gio * kwk::bench::LEGEND_LOAD_FACTOR; } 

  else  if (hname == "falcou-avx512")          { size =   6 * gio; }
  else  if (hname == "pata")                   { size =   1 * gio; }

  else  if (hname == "chaton")                 { size = 128 * mio; }
  else  if (hname == "sylvain-ThinkPad-T580")  { size =  32 * mio; }
  else  if (hname == "lapierre")               { size =  32 * mio; }
  else                                         { size =   1 * gio; }
  
  sutils::printer_t::head("Benchmark - transform_reduce, memory-bound", true);

  transform_reduce_test<DATA_TYPE>("Transform_reduce memory-bound"
                                  , "transform_reduce_memory-bound_" + kwk::bench::EVE_COMPILER_FLAG + ".bench"
                                  , reduce_func
                                  , 1 // neutral element for the reduce function
                                  , transform_func
                                  , transform_func
                                  , size
                                  );
};

#endif // KIWAKU_BUILD_BENCH