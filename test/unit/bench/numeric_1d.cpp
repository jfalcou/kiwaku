//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <cstdlib>
#include <kwk/context/sycl/context.hpp>
#include <kwk/context/sycl/internal/sycl_tools.hpp>
#include <kwk/algorithm/algos/numeric.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <numeric>
#include "include/benchmark.hpp"
#include "include/utils/utils.hpp"
#include <cmath>
#include <execution> // don't forget the -ltbb compiler flag


template<typename DATA_TYPE>
void transform_reduce_test(std::string const& bench_name
                          , std::string const& file_name
                          , auto reduce_func
                          , auto transform_func
                          , std::size_t const view_size
                          )
{
  // Original :
  // DATA_TYPE1 = int
  // DATA_TYPE2 = float

  const std::size_t input_size = view_size;
  std::vector<DATA_TYPE> input1;
  std::vector<DATA_TYPE> input2;
  input1.resize(input_size);
  input2.resize(input_size);

  DATA_TYPE init_value = 11;

  srand(89561221);
  const int max_number = 1 << 20;
  for (std::size_t i = 0; i < input_size; ++i)
  {
    input1[i] = rand() % max_number;
    input2[i] = rand() % max_number;
  }

  auto view1 = kwk::view{kwk::source = input1, kwk::of_size(input_size)};
  auto view2 = kwk::view{kwk::source = input2, kwk::of_size(input_size)};


  DATA_TYPE res_kwk_cpu, res_std, res_std_par, res_std_par_unseq, res_hand;

  auto fct_kwk_cpu = [&]() {
    res_kwk_cpu = kwk::transform_reduce(view1, view2, init_value, reduce_func, transform_func);
    return res_kwk_cpu; };

  auto fct_std = [&]()
  {
    res_std = std::transform_reduce ( input1.begin()
                                    , input1.end()
                                    , input2.begin()
                                    , init_value
                                    , reduce_func
                                    , transform_func
                                    );
    return res_std;
  };

  auto fct_std_par = [&]()
  {
    res_std_par = std::transform_reduce ( std::execution::par
                                        , input1.begin()
                                        , input1.end()
                                        , input2.begin()
                                        , init_value
                                        , reduce_func
                                        , transform_func
                                        );
    return res_std;
  };

  auto fct_std_par_unseq = [&]()
  {
    res_std_par_unseq = std::transform_reduce ( std::execution::par_unseq
                                              , input1.begin()
                                              , input1.end()
                                              , input2.begin()
                                              , init_value
                                              , reduce_func
                                              , transform_func
                                              );
    return res_std;
  };

  auto fct_hand = [&]() {
    res_hand = init_value;
    for (std::size_t i = 0; i < input_size; ++i)
    {
      res_hand = reduce_func(res_hand, transform_func(input1[i], input2[i]));
    }
    return res_hand;
  };

  std::string absolute_path = ""; // will output to "kiwaku_build"
  std::string final_fname = kwk::bench::fprefix() + file_name;

  kwk::bench::cbench_t b;
  b.start(absolute_path + final_fname, bench_name, "Processed elements, per second (higher is better)", view_size);
  b.run_function("std::execution::seq", fct_std); // std::transform_reduce, single thread on CPU
  b.run_function("std::execution::par", fct_std_par);
  b.run_function("std::execution::par_unseq", fct_std_par_unseq);

  // Don't forget -fsycl-targets=nvptx64-nvidia-cuda,x86_64 or spir64
  bool has_gpu = kwk::sycl::has_gpu();

  auto sycl_bench = [&](auto&& context, DATA_TYPE& return_) -> DATA_TYPE
  {
    auto fct_kwk_sycl_generic = [&]()
    {
      return_ = kwk::transform_reduce(context, view1, view2, init_value, reduce_func, transform_func);
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
    TTS_RELATIVE_EQUAL(return_, res_std, FLOAT_TOLERANCE_PERCENT_BENCH);

    // CPU
    sycl_bench(kwk::sycl::context{::sycl::cpu_selector_v}, return_);
    TTS_RELATIVE_EQUAL(return_, res_std, FLOAT_TOLERANCE_PERCENT_BENCH);
  }
  else // SYCL default context
  {
    DATA_TYPE return_;
    // CPU
    sycl_bench(kwk::sycl::default_context, return_);
    TTS_RELATIVE_EQUAL(return_, res_std, FLOAT_TOLERANCE_PERCENT_BENCH);
  }

  b.run_function("Kiwaku on CPU", fct_kwk_cpu);
  b.run_function("By hand on CPU", fct_hand);
  b.stop();

  TTS_RELATIVE_EQUAL(res_std_par      , res_std, FLOAT_TOLERANCE_PERCENT_BENCH);
  TTS_RELATIVE_EQUAL(res_std_par_unseq, res_std, FLOAT_TOLERANCE_PERCENT_BENCH);
  TTS_RELATIVE_EQUAL(res_kwk_cpu      , res_std, FLOAT_TOLERANCE_PERCENT_BENCH);
  TTS_RELATIVE_EQUAL(res_hand         , res_std, FLOAT_TOLERANCE_PERCENT_BENCH);
}



TTS_CASE("Benchmark - transform_reduce, compute-bound ")
{
  if (::kwk::bench::enable_global)
  {
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
    // auto reduce_func    = [](auto a, auto b) { return (((((((((((a + b) % 4096) % 2048) % 1024) % 512) % 256) % 128) % 64) % 32) % 16) % 8); };
    // auto transform_func = [](auto a, auto b) { return ((((((((((((a + b) % 131072) % 65536) % 32768) % 16384) % 8192) % 4096) % 2048) % 1024) % 512) % 256) % 128); };

    [[maybe_unused]] std::size_t kio = 1024 / (sizeof(DATA_TYPE) * 2);
    [[maybe_unused]] std::size_t mio = 1024 * kio;
    [[maybe_unused]] std::size_t gio = 1024 * mio;

    std::size_t size;
    std::string hname = sutils::get_host_name();
         if (hname == "parsys-legend")          { size =   1 * gio * kwk::bench::LEGEND_LOAD_FACTOR; } 
    else if (hname == "pata")                   { size =   1 * gio; }
    else if (hname == "chaton")                 { size = 128 * mio; }
    else if (hname == "sylvain-ThinkPad-T580")  { size =  32 * mio; }
    else if (hname == "lapierre")               { size =  32 * mio; }
    else                                        { size =   1 * gio; }

    sutils::printer_t::head("Benchmark - transform_reduce, compute-bound", true);

    transform_reduce_test<DATA_TYPE>("Transform_reduce compute-bound", "transform_reduce_compute-bound.bench", reduce_func, transform_func, size);
  }
  else
  {
    TTS_EQUAL(true, true);
  }
};



TTS_CASE("Benchmark - transform_reduce, memory-bound ")
{
  if (::kwk::bench::enable_global)
  {
    using DATA_TYPE = float;
    using DATA_TYPE = float;

    auto reduce_func    = [](auto a, auto b) { return (a + b); };
    auto transform_func = [](auto a, auto b) { return (a * b); };

    [[maybe_unused]] std::size_t kio = 1024 / (sizeof(DATA_TYPE) + sizeof(DATA_TYPE));
    [[maybe_unused]] std::size_t mio = 1024 * kio;
    [[maybe_unused]] std::size_t gio = 1024 * mio;

    std::size_t size;
    std::string hname = sutils::get_host_name();
         if (hname == "parsys-legend")          { size =   6 * gio * kwk::bench::LEGEND_LOAD_FACTOR; } 
    else if (hname == "pata")                   { size =   1 * gio; }
    else if (hname == "chaton")                 { size = 128 * mio; }
    else if (hname == "sylvain-ThinkPad-T580")  { size =  32 * mio; }
    else if (hname == "lapierre")               { size =  32 * mio; }
    else                                        { size =   1 * gio; }
    
    sutils::printer_t::head("Benchmark - transform_reduce, memory-bound", true);

    transform_reduce_test<DATA_TYPE>("Transform_reduce memory-bound", "transform_reduce_memory-bound.bench", reduce_func, transform_func, size);
  }
  else
  {
    TTS_EQUAL(true, true);
  }
};