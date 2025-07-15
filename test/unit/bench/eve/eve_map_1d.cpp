//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

#include "../include/benchmark.hpp"
#include "../include/utils/utils.hpp"

#if EVE_ENABLE_SYCL
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

#define ENABLE_CHECK true

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

  b.run_function_rpt("std::execution::seq", repeat_count, fct_std, [&]{ reset_data(data_out); }); // "std::for_each, single thread on CPU

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
    b.run_function_rpt("std::execution::unseq", repeat_count, fct_std_unseq, [&]{ reset_data(data_out); });
    #if ENABLE_CHECK
      TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);
    #endif
    b.run_function_rpt("std::execution::par", repeat_count, fct_std_par, [&]{ reset_data(data_out); });
    #if ENABLE_CHECK
      TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);
    #endif
    b.run_function_rpt("std::execution::par_unseq", repeat_count, fct_std_par_unseq, [&]{ reset_data(data_out); });
    #if ENABLE_CHECK
      TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);
    #endif
  #endif

  #if EVE_ENABLE_SYCL
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
      b.run_function("Kiwaku SYCL on " + context.get_device_name(), fct_kwk_sycl_generic, [&]{ reset_data(data_out); });
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
    b.run_function_rpt(kwk::bench::EVE_BACKEND_NAME, repeat_count, fct_kwk_sycl_generic, [&]{ reset_data(data_out); });
    return return_;
  };

  DATA_TYPE return_;
  // EVE SIMD
  generic_bench(kwk::simd, return_);
  #if ENABLE_CHECK
    TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);
  #endif

  b.run_function_rpt("Kiwaku on CPU", repeat_count, fct_kwk_cpu, [&]{ reset_data(data_out); });
  #if ENABLE_CHECK
    TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);
  #endif

  b.run_function_rpt("By hand on CPU 1", repeat_count, fct_hand1, [&]{ reset_data(data_out); });
  // b.run_function_rpt("By hand on CPU 2", repeat_count, fct_hand2, [&]{ reset_data(data_out); });
  // b.run_function_rpt("By hand on CPU 3", repeat_count, fct_hand3, [&]{ reset_data(data_out); });
  #if ENABLE_CHECK
    TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);
  #endif

  for (std::size_t i = 0; i < 30; ++i) { std::cout << data_out_truth[i] << " "; }
  std::cout << "\n";
  b.stop();
}



TTS_CASE("Benchmark - for_each, compute-bound_trigo")
{
  if (::kwk::bench::enable_global)
  {
    ::kwk::bench::get_eve_compiler_flag();

    using DATA_TYPE = float;

    // const DATA_TYPE PI = static_cast<DATA_TYPE>(M_PI);

    auto map_func = [=](DATA_TYPE const& in, DATA_TYPE& out)
    {
      // out = ::std::cos(in) + ::std::sin(in);
      out = ::std::cos(in) * ::std::cos(in) + ::std::sin(in) * ::std::sin(in) + ::std::cos(in) * ::std::cos(in) + ::std::sin(in) * ::std::sin(in)
          + ::std::cos(in) * ::std::cos(in) + ::std::sin(in) * ::std::sin(in) + ::std::cos(in) * ::std::cos(in) + ::std::sin(in) * ::std::sin(in);
    };

    auto map_func_eve = [=](auto const& in, auto& out)
    {
      // out = ::eve::cos(in) + ::eve::sin(in);
      out = ::eve::cos(in) * ::eve::cos(in) + ::eve::sin(in) * ::eve::sin(in) + ::eve::cos(in) * ::eve::cos(in) + ::eve::sin(in) * ::eve::sin(in)
          + ::eve::cos(in) * ::eve::cos(in) + ::eve::sin(in) * ::eve::sin(in) + ::eve::cos(in) * ::eve::cos(in) + ::eve::sin(in) * ::eve::sin(in);
    };

    // auto map_func = [=](DATA_TYPE const& in, DATA_TYPE& out)
    // {
    //   out = static_cast<DATA_TYPE>(
    //     std::cos(
    //       std::sin(
    //         std::cos(
    //           std::sin(
    //             std::cos(
    //               std::sin(
    //                 static_cast<float>(in * 11.f) * 7.f
    //                       ) * 3.8f
    //                     ) / 1.12f
    //                   ) * 3.17874f
    //                 ) / 8.7f
    //               ) * 9.48f
    //             ) / 89.647681f
    //           ) * 1.8746221f;
    // };

    // auto map_func_eve = [=](DATA_TYPE const& in, DATA_TYPE& out)
    // {
    //   out = static_cast<DATA_TYPE>(
    //     eve::cos(
    //       eve::sin(
    //         eve::cos(
    //           eve::sin(
    //             eve::cos(
    //               eve::sin(
    //                 static_cast<float>(in * 11.f) * 7.f
    //                       ) * 3.8f
    //                     ) / 1.12f
    //                   ) * 3.17874f
    //                 ) / 8.7f
    //               ) * 9.48f
    //             ) / 89.647681f
    //           ) * 1.8746221f;
    // };

    [[maybe_unused]] std::size_t kio = 1024 / (sizeof(DATA_TYPE) * 1); // input + output
    [[maybe_unused]] std::size_t mio = 1024 * kio;
    [[maybe_unused]] std::size_t gio = 1024 * mio;

    std::size_t total_size;
    std::size_t L2_size; // L2 cache size, in bytes
    std::string hname = sutils::get_host_name();
    // Total data to process
         if (hname == "parsys-legend")          { L2_size = 512 * kio; total_size = 1 * gio * kwk::bench::LEGEND_LOAD_FACTOR; } 

    else if (hname == "pata")                   { total_size = 128 * mio; L2_size = total_size; }
    else if (hname == "chaton")                 { total_size = 128 * mio; L2_size = total_size; }
    // else if (hname == "chaton")                 { L2_size = 256 * kio; total_size = 128 * mio; }
    else if (hname == "sylvain-ThinkPad-T580")  { L2_size = 256 * kio; total_size = 8 * mio; }

    // 512 kio par L2
    else if (hname == "falcou-avx512")          { total_size = 512 * mio;  L2_size = total_size; //128 * kio; 
                                                  std::cout << "OK, Used computer: falcou-avx512\n"; }

    // La Pierre
    else if (hname == "lapierre")               { L2_size = 128 * mio; total_size = 256 * mio; }
    else                                        { L2_size = 256 * kio; total_size = 8 * mio; }

    std::size_t reps = total_size / L2_size; // Number of repetitions


    std::string l2_str = std::to_string(L2_size / kio);


    sutils::printer_t::head("Benchmark - for_each, compute-bound_trigo (L2 " + l2_str + ")", true);

    for_each_test<DATA_TYPE>( "for_each compute-bound_trigo"
                            , "for_each_compute-bound_trigo_" + kwk::bench::EVE_COMPILER_FLAG + "_L2-" + l2_str + ".bench"
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


// Legend:
// L1d: 512 KiB (16 instances)
// L1i: 512 KiB (16 instances)
// L2:  8 MiB (16 instances) 512 KB (per core)
// L3:  64 MiB (2 instances)

// Chaton
// L1d cache: 128 KiB
// L1i cache: 128 KiB
// L2 cache:  1 MiB
// L3 cache:  8 MiB

// La Pierre
// Caches (sum of all):
// L1d: 128 KiB (4 instances)
// L1i: 128 KiB (4 instances)
// L2:  1 MiB (4 instances)
// L3:  6 MiB (1 instance)

// From https://www.techpowerup.com/cpu-specs/core-i5-4670k.c1469
// Cache L1: 64 KB (per core)
// Cache L2: 256 KB (per core)
// Cache L3: 6 MB (shared)

// falcou-avx512
// Caches (sum of all):      
// L1d:                    384 KiB (8 instances)
// L1i:                    256 KiB (8 instances)
// L2:                     4 MiB (8 instances)
// L3:                     16 MiB (1 instance)


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

    [[maybe_unused]] std::size_t kio = 1024 / (sizeof(DATA_TYPE) * 1); // input + output
    [[maybe_unused]] std::size_t mio = 1024 * kio;
    [[maybe_unused]] std::size_t gio = 1024 * mio;

    std::size_t total_size;
    std::size_t L2_size; // L2 cache size, in bytes
    std::string hname = sutils::get_host_name();
    // Total data to process
         if (hname == "parsys-legend")          { L2_size = 512 * kio; total_size = 1 * gio * kwk::bench::LEGEND_LOAD_FACTOR; } 
    else if (hname == "pata")                   { total_size = 2 * gio; L2_size = total_size; }
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






// L2_size(32768) repeat_count(1024)

// ============================
// Start benchmark environment:
//     for_each compute-bound_trigo
//     falcou-avx512_2025-07-12_22h53m25s_for_each_compute-bound_trigo_-march=skylake-avx512_L2-128.bench
//     in directory ../kiwaku_bench_results/

// Benchmarking  std::execution::seq:
//     995 995 995 995   sum_ret(16384)

// Benchmarking  std::execution::par:
//     98 94 94 95   sum_ret(16384)

// Benchmarking  std::execution::par_unseq:
//     95 95 96 95   sum_ret(16384)

// Benchmarking  Kiwaku SIMD -march=skylake-avx512:
//     160 160 160 160   sum_ret(16384)

// Benchmarking  Kiwaku on CPU:
//     988 988 988 988   sum_ret(16384)

// Benchmarking  By hand on CPU 1:
//     1015 995 995 995   sum_ret(16384)


// L2_size(33554432) repeat_count(1)

// ============================
// Start benchmark environment:
//     for_each compute-bound_trigo
//     falcou-avx512_2025-07-12_22h54m07s_for_each_compute-bound_trigo_-march=skylake-avx512_L2-131072.bench
//     in directory ../kiwaku_bench_results/

// Benchmarking  std::execution::seq:
//     975 976 975 975   sum_ret(16)

// Benchmarking  std::execution::par:
//     80 76 76 77   sum_ret(16)

// Benchmarking  std::execution::par_unseq:
//     77 77 77 77   sum_ret(16)

// Benchmarking  Kiwaku SIMD -march=skylake-avx512:
//     157 157 157 157   sum_ret(16)

// Benchmarking  Kiwaku on CPU:
//     969 969 969 969   sum_ret(16)

// Benchmarking  By hand on CPU 1:
//     975 996 975 975   sum_ret(16)


// L2_size(65536) repeat_count(512)

// ============================
// Start benchmark environment:
//     for_each compute-bound_trigo
//     chaton_2025-07-12_23h18m50s_for_each_compute-bound_trigo_-mavx2_-mfma_L2-256.bench
//     in directory ../kiwaku_bench_results/

// Benchmarking  std::execution::seq:
//     1243 1238 1237 1235   sum_ret(8192)

// Benchmarking  std::execution::par:
//     248 408 267 244   sum_ret(8192)

// Benchmarking  std::execution::par_unseq:
//     430 246 247 247   sum_ret(8192)

// Benchmarking  Kiwaku SIMD -mavx2_-mfma:
//     272 271 271 272   sum_ret(8192)

// Benchmarking  Kiwaku on CPU:
//     1211 1216 1206 1208   sum_ret(8192)

// Benchmarking  By hand on CPU 1:
//     1235 1236 1236 1237   sum_ret(8192)


// L2_size(33554432) repeat_count(1)

// ============================
// Start benchmark environment:
//     for_each compute-bound_trigo
//     chaton_2025-07-12_23h20m38s_for_each_compute-bound_trigo_-mavx2_-mfma_L2-131072.bench
//     in directory ../kiwaku_bench_results/

// Benchmarking  std::execution::seq:
//     1244 1241 1243 1243   sum_ret(16)

// Benchmarking  std::execution::par:
//     232 234 230 231   sum_ret(16)

// Benchmarking  std::execution::par_unseq:
//     248 263 233 234   sum_ret(16)

// Benchmarking  Kiwaku SIMD -mavx2_-mfma:
//     272 307 271 286   sum_ret(16)

// Benchmarking  Kiwaku on CPU:
//     1218 1217 1212 1209   sum_ret(16)

// Benchmarking  By hand on CPU 1:
//     1240 1240 1240 1239   sum_ret(16)