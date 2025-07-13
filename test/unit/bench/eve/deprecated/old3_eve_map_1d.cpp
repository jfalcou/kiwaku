//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/context/cpu/context.hpp>
#include <kwk/context/eve/context.hpp>
// #include <kwk/context/sycl/internal/sycl_tools.hpp>
#include <cstdlib>
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <numeric>
#include <optional>
#include "../include/benchmark.hpp"
#include "../include/utils/utils.hpp"
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

  auto start_data_truth = &data_out_truth[0];
  auto std_map_func = [&](auto const& e)
  {
    std::size_t index = &e - start_data_truth;
    return map_func(e, data_out_truth[index]); // e = data_in[index]
  };

  auto fct_std = [&]()
  {
    std::for_each(data_out_truth.begin(), data_out_truth.end(), std_map_func);
    return data_out_truth[L2_size / 2];
  };

  auto fct_hand = [&]()
  {
    for (std::size_t i = 0; i < L2_size; ++i)
    {
      map_func(data_in[i], data_out[i]);
    }
  return data_out[L2_size / 2];
  };

  kwk::bench::cbench_t b;
  std::string absolute_path = ""; // will output to "kiwaku_build"

  b.start(absolute_path + kwk::bench::fprefix() + file_name, bench_name, "Processed elements, per second (higher is better)", L2_size * repeat_count);
  // b.set_iterations(1);

  b.run_function_rpt("std::execution::seq", repeat_count, fct_std, [&]{ reset_data(data_out_truth); }); // "std::for_each, single thread on CPU

  #if ENABLE_TBB
    auto start_data = &data_out[0];
    // inline ? Is function call expensive?
    auto std_par_map_func = [&](auto const& e) 
    {
      std::size_t index = &e - start_data;
      return map_func(e, data_out[index]); // e = data_in[index]
    };
    // Don't forget the -ltbb compiler flag
    auto fct_std_par = [&]()
    {
      std::for_each(std::execution::par, data_out.begin(), data_out.end(), std_par_map_func);
      return data_out[L2_size / 2];
    };

    // Don't forget the -ltbb compiler flag
    auto fct_std_par_unseq = [&]()
    {
      std::for_each(std::execution::par_unseq, data_out.begin(), data_out.end(), std_par_map_func);
      return data_out[L2_size / 2];
    };
    b.run_function_rpt("std::execution::par", repeat_count, fct_std_par, [&]{ reset_data(data_out); });
    TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);
    b.run_function_rpt("std::execution::par_unseq", repeat_count, fct_std_par_unseq, [&]{ reset_data(data_out); });
    TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);
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
  TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);

  b.run_function_rpt("Kiwaku on CPU", repeat_count, fct_kwk_cpu, [&]{ reset_data(data_out); });
  TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);

  b.run_function_rpt("By hand on CPU", repeat_count, fct_hand, [&]{ reset_data(data_out); });
  TTS_ALL_RELATIVE_EQUAL(data_out, data_out_truth, 1);

  for (std::size_t i = 0; i < 30; ++i) { std::cout << data_out_truth[i] << " "; }
  std::cout << "\n";
  b.stop();
}



TTS_CASE("Benchmark - for_each, compute-bound_trigo")
{
  if (::kwk::bench::enable_global)
  {
    using DATA_TYPE = float;

    // adding "-> DATA_TYPE" leads to segmentation fault.
    // auto map_func = [=](DATA_TYPE& item)
    // {
    //   // TODO: test with multiplications and divisions
    //   // Modulo may imply thread divergence, which would impair GPU performance.
    //   item = static_cast<DATA_TYPE>(
    //     std::cos(
    //       std::sin(
    //         std::cos(
    //           std::sin(
    //             std::cos(
    //               std::sin(
    //                 static_cast<float>(item * 11.f) * 7.f
    //                       ) * 3.8f
    //                     ) / 1.12f
    //                   ) * 3.17874f
    //                 ) / 8.7f
    //               ) * 9.48f
    //             ) / 89.647681f
    //           ) * 1.8746221f;
    // };

    const DATA_TYPE PI = static_cast<DATA_TYPE>(M_PI);

    auto map_func = [=](DATA_TYPE const& in, DATA_TYPE& out)
    {
      out =
      (::std::cos(
        (::std::sin(
          (::std::cos(
            (::std::sin(
              (::std::cos(
                (::std::sin(
                  in
                ) + 1) * PI
              ) + 1) * PI
            ) + 1) * PI
          ) + 1) * PI
        ) + 1) * PI
      ) + 1) * PI;
    };

    auto map_func_eve = [=](auto const& in, auto& out)
    {
      out =
      (::std::cos(
        (::std::sin(
          (::std::cos(
            (::std::sin(
              (::std::cos(
                (::std::sin(
                  in
                ) + 1) * PI
              ) + 1) * PI
            ) + 1) * PI
          ) + 1) * PI
        ) + 1) * PI
      ) + 1) * PI;
    };
    // auto map_func_eve = [=](auto& item)
    // {
    //   // TODO: test with multiplications and divisions
    //   // Modulo may imply thread divergence, which would impair GPU performance.
    //   item =
    //   ::eve::cos(
    //     ::eve::sin(
    //       ::eve::cos(
    //         ::eve::sin(
    //           ::eve::cos(
    //             ::eve::sin(
    //               item * static_cast<DATA_TYPE>(1.425)
    //             ) * static_cast<DATA_TYPE>(1.458)
    //           ) / static_cast<DATA_TYPE>(1.578)
    //         ) * static_cast<DATA_TYPE>(1.98794)
    //       ) / static_cast<DATA_TYPE>(1.578)
    //     ) * static_cast<DATA_TYPE>(1.98794)
    //   ) / static_cast<DATA_TYPE>(1.87961);
    // };

    [[maybe_unused]] std::size_t kio = 1024 / (sizeof(DATA_TYPE) * 1); // input + output
    [[maybe_unused]] std::size_t mio = 1024 * kio;
    [[maybe_unused]] std::size_t gio = 1024 * mio;

    std::size_t total_size;
    std::size_t L2_size; // L2 cache size, in bytes
    std::string hname = sutils::get_host_name();
    // Total data to process
         if (hname == "parsys-legend")          { L2_size = 512 * kio; total_size = 1 * gio * kwk::bench::LEGEND_LOAD_FACTOR; } 
    else if (hname == "pata")                   { L2_size = 256 * kio; total_size = 8 * mio; }
    else if (hname == "chaton")                 { L2_size = 256 * kio; total_size = 8 * mio; }
    else if (hname == "sylvain-ThinkPad-T580")  { L2_size = 256 * kio; total_size = 8 * mio; }
    else if (hname == "lapierre")               { L2_size = 256 * kio; total_size = 8 * mio; }
    else                                        { L2_size = 256 * kio; total_size = 8 * mio; }

    std::size_t reps = total_size / L2_size; // Number of repetitions

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


    sutils::printer_t::head("Benchmark - for_each, compute-bound_trigo", true);

    for_each_test<DATA_TYPE>( "for_each compute-bound_trigo"
                            , "for_each_compute-bound_trigo_" + kwk::bench::EVE_COMPILER_FLAG + ".bench"
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

// TTS_CASE("Benchmark - for_each, memory-bound")
// {
//   if (::kwk::bench::enable_global)
//   {
//     using DATA_TYPE = float;

//     // FMA
//     auto map_func     = [=](DATA_TYPE& d) { d = d * static_cast<DATA_TYPE>(2.f); };
//     auto map_func_eve = [=](DATA_TYPE& d) { d = d * static_cast<DATA_TYPE>(2.f); };

//     [[maybe_unused]] std::size_t kio = 1024 / sizeof(DATA_TYPE);
//     [[maybe_unused]] std::size_t mio = 1024 * kio;
//     [[maybe_unused]] std::size_t gio = 1024 * mio;

//     std::size_t size;
//     std::string hname = sutils::get_host_name();
//          if (hname == "parsys-legend")          { size =   1 * gio * kwk::bench::LEGEND_LOAD_FACTOR; } 
//     else if (hname == "pata")                   { size =  64 * mio; }
//     else if (hname == "chaton")                 { size =  64 * mio; }
//     else if (hname == "sylvain-ThinkPad-T580")  { size =  64 * mio; }
//     else if (hname == "lapierre")               { size =  64 * mio; }
//     else                                        { size =  64 * mio; }

//     sutils::printer_t::head("Benchmark - for_each, memory-bound", true);

//     for_each_test<DATA_TYPE>( "for_each memory-bound"
//                             , "for_each_memory-bound_" + kwk::bench::EVE_COMPILER_FLAG + ".bench"
//                             , map_func
//                             , map_func_eve
//                             , size
//                             );
//     std::cout << "\n\n";
//   }
//   else
//   {
//     TTS_EQUAL(true, true);
//   }
// };




