//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

#include "../include/benchmark.hpp"
#include "../include/utils/utils.hpp"

#if KIWAKU_BENCH_SYCL
  #include <kwk/context/sycl/context.hpp>
#endif

#include <kwk/context/cpu/context.hpp>
#include <kwk/context/eve/context.hpp>
// #include <kwk/context/sycl/internal/sycl_tools.hpp>
#include <cstdlib>
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <numeric>
#include <optional>
#include <cmath>
#include <execution> // don't forget the -ltbb compiler flag
#include <eve/module/math.hpp>

template<typename DATA_TYPE>
void for_each_test( std::string const& bench_name
                  , std::string const& file_name
                  , auto map_func
                  , auto map_func_eve
                  , std::size_t const L2_size
                  , std::size_t const repeat_count
                  )
{
  std::cout << "L2_size(" << L2_size << ") repeat_count(" << repeat_count << ")\n";
  std::vector<DATA_TYPE> data_in;
  std::vector<DATA_TYPE> data_out;
  std::vector<DATA_TYPE> data_out_truth; // remember result for checking other values

  // data_size doit être plus petit que la taille du cache
  data_in.resize(L2_size);
  data_out.resize(L2_size);
  data_out_truth.resize(L2_size);

  double data_size_bytes = L2_size * repeat_count * sizeof(DATA_TYPE) * 2; // in + out


  auto reset_data = [&](std::vector<DATA_TYPE>& vect)
  {
    // std::string token = "salu sa va";
    // std::random_device rd{token};
    std::mt19937 gen(78512); // rd()
    std::uniform_real_distribution<DATA_TYPE> dis(0, 2.0 * M_PI);
    // srand(9546312);
    // DATA_TYPE min_value = 0;
    // DATA_TYPE max_value = 1.9 * M_PI; // Between 0 and 2PI
    for (std::size_t i = 0; i < L2_size; ++i)
      {
        vect[i] = dis(gen); //kwk::bench::random_float<DATA_TYPE>(min_value, max_value);
      }
  };

  auto kwk_in   = kwk::view{kwk::source = data_in.data()  , kwk::of_size(L2_size)};
  auto kwk_out  = kwk::view{kwk::source = data_out.data() , kwk::of_size(L2_size)};

  auto fct_kwk_cpu = [&]()
  {
    kwk::for_each(::kwk::cpu, map_func, kwk_in, kwk_out);
    return kwk_out(L2_size / 2);
  };

  // auto start_data_truth = &data_out_truth[0];
  auto std_map_func = [&](auto const& in)
  {
    // std::size_t index = &e - start_data_truth;
    int index = &in - &data_in[0];
    return map_func(in, data_out[index]); // e = data_in[index]
  };

  auto fct_std = [&]()
  {
    std::for_each(data_in.begin(), data_in.end(), std_map_func);
    return data_out[L2_size / 2];
  };

  auto fct_hand1 = [&]()
  {
    for (std::size_t i = 0; i < L2_size; ++i) { map_func(data_in[i], data_out[i]); }
    return data_out[L2_size / 2];
  };

  // auto fct_hand2 = [&]()
  // {
  //   for (std::size_t i = 0; i < L2_size; ++i)
  //   {
  //     int index = &data_in[i] - &data_in[0];
  //     map_func(data_in[i], data_out[index]);
  //   }
  //   return data_out[L2_size / 2];
  // };

  // auto fct_hand3 = [&]()
  // {
  //   for (auto const& e : data_in)
  //   {
  //     int index = &e - &data_in[0];
  //     map_func(e, data_out[index]);
  //   }
  //   return data_out[L2_size / 2];
  // };


  kwk::bench::cbench_t b;
  std::string absolute_path = ""; // will output to "kiwaku_build"

  b.start(absolute_path + kwk::bench::fprefix() + file_name, bench_name, "Processed elements, per second (higher is better)", L2_size * repeat_count);
  // b.set_iterations(1);

  b.run_function_rpt_bwidth("std::execution::seq", repeat_count, fct_std, [&]{ reset_data(data_out); }, data_size_bytes); // "std::for_each, single thread on CPU

  for (std::size_t i = 0; i < data_out_truth.size(); ++i)
  {
    data_out_truth[i] = data_out[i];
  }

  #if ENABLE_TBB
    auto start_data = &data_in[0];
    // inline ? Is function call expensive?
    auto std_par_map_func = [&](auto const& in) 
    {
      std::size_t index = &in - start_data;
      return map_func(in, data_out[index]); // e = data_in[index]
    };
    // Don't forget the -ltbb compiler flag
    auto fct_std_unseq = [&]()
    {
      std::for_each(std::execution::unseq, data_in.begin(), data_in.end(), std_par_map_func);
      return data_out[L2_size / 2];
    };

    // Don't forget the -ltbb compiler flag
    auto fct_std_par = [&]()
    {
      std::for_each(std::execution::par, data_in.begin(), data_in.end(), std_par_map_func);
      return data_out[L2_size / 2];
    };

    // Don't forget the -ltbb compiler flag
    auto fct_std_par_unseq = [&]()
    {
      std::for_each(std::execution::par_unseq, data_in.begin(), data_in.end(), std_par_map_func);
      return data_out[L2_size / 2];
    };
    b.run_function_rpt_bwidth("std::execution::unseq", repeat_count, fct_std_unseq, [&]{ reset_data(data_out); }, data_size_bytes);
    #if ENABLE_CHECK
      TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);
    #endif
    b.run_function_rpt_bwidth("std::execution::par", repeat_count, fct_std_par, [&]{ reset_data(data_out); }, data_size_bytes);
    #if ENABLE_CHECK
      TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);
    #endif
    b.run_function_rpt_bwidth("std::execution::par_unseq", repeat_count, fct_std_par_unseq, [&]{ reset_data(data_out); }, data_size_bytes);
    #if ENABLE_CHECK
      TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);
    #endif
  #endif

  #if KIWAKU_BENCH_SYCL
    // Don't forget -fsycl-targets=nvptx64-nvidia-cuda,x86_64 or spir64
    bool has_gpu = kwk::sycl::has_gpu();

    auto sycl_bench = [&](auto&& context, DATA_TYPE& return_) -> DATA_TYPE
    {
      auto fct_kwk_sycl_generic = [&]()
      {
        kwk::for_each(context, map_func, kwk_in, kwk_out);
        return_ = kwk_out(L2_size / 2);
        return return_;
      };
      b.run_function_rpt_bwidth("Kiwaku SYCL on " + context.get_device_name(), repeat_count, fct_kwk_sycl_generic, [&]{ reset_data(data_out); }, data_size_bytes);
      return return_;
    };

    // Execute SYCL benchmark on GPU and CPU
    if (has_gpu)
    {
      DATA_TYPE return_;
      // GPU
      sycl_bench(kwk::sycl::context{::sycl::gpu_selector_v}, return_);
      TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);
      // CPU
      sycl_bench(kwk::sycl::context{::sycl::cpu_selector_v}, return_);
      TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);
    }
    else // SYCL default context
    {
      DATA_TYPE return_;
      // CPU
      sycl_bench(kwk::sycl::default_context, return_);
      TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);
    }
  #endif


  auto generic_bench = [&](auto&& context, DATA_TYPE& return_) -> DATA_TYPE
  {
    auto fct_kwk_sycl_generic = [&]()
    {
      kwk::for_each(context, map_func_eve, kwk_in, kwk_out);
      return_ = kwk_out(L2_size / 2);
      return return_;
    };
    b.run_function_rpt_bwidth(kwk::bench::EVE_BACKEND_NAME, repeat_count, fct_kwk_sycl_generic, [&]{ reset_data(data_out); }, data_size_bytes);
    return return_;
  };

  DATA_TYPE return_;
  // EVE SIMD
  generic_bench(kwk::simd, return_);
  #if ENABLE_CHECK
    TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);
  #endif

  b.run_function_rpt_bwidth("Kiwaku on CPU", repeat_count, fct_kwk_cpu, [&]{ reset_data(data_out); }, data_size_bytes);
  #if ENABLE_CHECK
    TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);
  #endif

  b.run_function_rpt_bwidth("By hand on CPU 1", repeat_count, fct_hand1, [&]{ reset_data(data_out); }, data_size_bytes);
  // b.run_function_rpt_bwidth("By hand on CPU 2", repeat_count, fct_hand2, [&]{ reset_data(data_out); }, data_size_bytes);
  // b.run_function_rpt_bwidth("By hand on CPU 3", repeat_count, fct_hand3, [&]{ reset_data(data_out); }, data_size_bytes);
  #if ENABLE_CHECK
    TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);
  #endif

  for (std::size_t i = 0; i < 30; ++i) { std::cout << data_out_truth[i] << " "; }
  std::cout << "\n";
  b.stop();
}



TTS_CASE("Benchmark - for_each, memory-bound")
{
  if (::kwk::bench::enable_global)
  {
    ::kwk::bench::get_eve_compiler_flag();

    using DATA_TYPE = float;

    auto map_func = [=](DATA_TYPE const& in, DATA_TYPE& out)
    {
      out = in + 2;
    };

    auto map_func_eve = [=](auto const& in, auto& out)
    {
      out = in + 2;
    };

    [[maybe_unused]] std::size_t kio = 1024 / (sizeof(DATA_TYPE) * 2); // input + output
    [[maybe_unused]] std::size_t mio = 1024 * kio;
    [[maybe_unused]] std::size_t gio = 1024 * mio;

    std::size_t total_size;
    std::size_t L2_size; // L2 cache size, in bytes
    std::string hname = sutils::get_host_name();
    // Total data to process
         if (hname == "parsys-legend")          { total_size = 6 * gio * kwk::bench::LEGEND_LOAD_FACTOR; L2_size = total_size;  } 

        //  if (hname == "parsys-legend")          { L2_size = 512 * kio; total_size = 6 * gio * kwk::bench::LEGEND_LOAD_FACTOR; } 

    else if (hname == "pata")                   { total_size = 1 * gio; L2_size = total_size; }
    else if (hname == "chaton")                 { total_size = 2 * gio; L2_size = total_size; }
    else if (hname == "sylvain-ThinkPad-T580")  { L2_size = 256 * kio; total_size = 8 * mio; }

    // 512 kio par L2
    else if (hname == "falcou-avx512")          { total_size = 2 * gio; L2_size = total_size; // memory-bound
                                                  std::cout << "OK, Used computer: falcou-avx512\n"; }

    // La Pierre
    else if (hname == "lapierre")               { L2_size = 128 * mio; total_size = 256 * mio; }
    else                                        { L2_size = 256 * kio; total_size = 8 * mio; }

    std::size_t reps = total_size / L2_size; // Number of repetitions

    std::string l2_str = std::to_string(L2_size / kio);

    sutils::printer_t::head("Benchmark - for_each, memory-bound (L2 " + l2_str + ")", true);

    for_each_test<DATA_TYPE>( "for_each memory-bound"
                            , "for_each_memory-bound_" + kwk::bench::EVE_COMPILER_FLAG + "_L2-" + l2_str + ".bench"
                            , map_func
                            , map_func_eve
                            , L2_size
                            , reps
                            );
    std::cout << "\n\n";
  }
  else
  {
    TTS_EQUAL(true, true);
  }
};



