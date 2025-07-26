//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

// export KIWAKU_BUILD_BENCH="ON"
// export KIWAKU_BENCH_EVE="ON"
// export KIWAKU_BENCH_SYCL="OFF"
// export KIWAKU_BENCH_MTHREAD="OFF"

#if KIWAKU_BUILD_BENCH

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
  constexpr bool enable_check = ENABLE_CHECK;

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
    std::mt19937 gen(78512); // rd()
    std::uniform_real_distribution<DATA_TYPE> dis(0, 2.0 * M_PI);
    for (std::size_t i = 0; i < L2_size; ++i)
      {
        vect[i] = dis(gen);
      }
  };

  // Kiwaku views
  auto kwk_in   = kwk::view{kwk::source = data_in.data()  , kwk::of_size(L2_size)};
  auto kwk_out  = kwk::view{kwk::source = data_out.data() , kwk::of_size(L2_size)};


  // Benchmark initialization
  kwk::bench::cbench_t b;
  b.start(kwk::bench::fprefix() + file_name, bench_name, "Processed elements, per second (higher is better)", L2_size * repeat_count);
  // b.set_iterations(1);


  // ====== Generic std function, used later ======
  auto fct_std_for_each = [&](auto const& policy)
  {
    auto start_data = &data_in[0];
    // std::execution::unseq
    // Is function call expensive?
    std::for_each ( policy
                  , data_in.begin()
                  , data_in.end()
                  , [&](auto const& in) 
                    {
                      int index = &in - start_data;
                      return map_func(in, data_out[index]); // e = data_in[index]
                    }
                  );
    return data_out[L2_size / 2];
  };

  // ====== Generic std benchmark ======
  auto bench_std = [&](auto const& policy, std::string policy_name)
  {
    b.run_function_rpt( policy_name
                      , repeat_count
                      , [&]{ return fct_std_for_each(policy); }
                      , [&]{ reset_data(data_out); }
                      );
  };

  // ====== Generic Kiwaku benchmark ======
  // -> DATA_TYPE
  auto bench_kiwaku = [&](auto&& context, std::string context_name, auto map_func_)
  {
    DATA_TYPE return_;
    auto fct_kwk_sycl_generic = [&]()
    {
      kwk::for_each(context, map_func_, kwk_in, kwk_out);
      return_ = kwk_out(L2_size / 2);
      return return_;
    };
    b.run_function_rpt(context_name, repeat_count, fct_kwk_sycl_generic, [&]{ reset_data(data_out); });
    // return return_;
  };


  // ====== hand-written sequential ======
  auto fct_hand = [&]()
  {
    for (std::size_t i = 0; i < L2_size; ++i) { map_func(data_in[i], data_out[i]); }
    return data_out[L2_size / 2];
  };
  b.run_function_rpt("hand CPU", repeat_count, fct_hand, [&]{ reset_data(data_out); });

  // Remember output for later verification
  for (std::size_t i = 0; i < data_out_truth.size(); ++i) { data_out_truth[i] = data_out[i]; }


  // ====== Kiwaku CPU context ======
  bench_kiwaku(::kwk::cpu, "kwk CPU", map_func);
  if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);


  // ====== std sequential ======
  bench_std(std::execution::seq, "seq");
  if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);


  // ====== std unsequenced ======
  bench_std(std::execution::unseq, "unseq");
  if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);



  // Don't forget the -ltbb compiler flag
  #if ENABLE_TBB && KIWAKU_BENCH_MTHREAD

    // ====== std parallel ======
    bench_std(std::execution::par, "par");
    if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);

    // ====== std parallel unsequenced ======
    bench_std(std::execution::par_unseq, "par_unseq");
    if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);
  #endif



  // ====== SYCL ======
  #if KIWAKU_BENCH_SYCL

    // Don't forget -fsycl-targets=nvptx64-nvidia-cuda,x86_64 or spir64
    bool has_gpu = kwk::sycl::has_gpu();

    // Execute SYCL benchmark on GPU and CPU
    if (has_gpu)
    {
      // ====== Kiwaku SYCL GPU ======
      auto& ctx = kwk::sycl::context{::sycl::gpu_selector_v};
      bench_kiwaku(ctx, "kwk SYCL " + ctx.get_device_name(), map_func);
      if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);

      #if KIWAKU_BENCH_MTHREAD
        // ====== Kiwaku SYCL CPU ======
        auto& ctx = kwk::sycl::context{::sycl::cpu_selector_v};
        bench_kiwaku(ctx, "kwk SYCL " + ctx.get_device_name(), map_func);
        if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);
      #endif
    }
    else
    {
      #if KIWAKU_BENCH_MTHREAD
        // ====== CPU ======
        auto& ctx = kwk::sycl::default_context;
        bench_kiwaku(ctx, "kwk SYCL " + ctx.get_device_name(), map_func);
        if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);
      #endif
    }
  #endif


  // EVE SIMD
  #if KIWAKU_BENCH_EVE
    bench_kiwaku(::kwk::simd, "kwk SIMD", map_func_eve);
    if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);
  #endif



  for (std::size_t i = 0; i < 30; ++i) { std::cout << data_out_truth[i] << " "; }
  std::cout << "\n";
  b.stop();
}



TTS_CASE("Benchmark - for_each, compute-bound_trigo")
{

  // (a + ib) * (c + id) = ac + iad + ibc -bd = ac - bd + i(ad + bc)

  // (a + ib) * (a + ib) = aa - bb + i(aa + ba)
  // std::size_t repeat_inner = 50;

  // auto map_v2 = [=](DATA_TYPE& re, DATA_TYPE& im) {
  //   for (std::size_t i = 0; i < repeat_inner; ++i) {
  //     re = re * re - im * im;
  //     im = re * re + im * im;
  //   }
  // };


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
    else if (hname == "chaton")                 { total_size = 32 * mio; L2_size = total_size; }
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

    [[maybe_unused]] std::size_t kio = 1024 / (sizeof(DATA_TYPE) * 1); // Only one item for (read + (read + write))
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


#endif // KIWAKU_BUILD_BENCH