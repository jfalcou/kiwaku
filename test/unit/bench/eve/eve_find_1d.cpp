//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
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
#include "../include/benchmark.hpp"
#include "../include/utils/utils.hpp"
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

  const std::size_t d0 = view_size;
  const std::size_t input_size = d0;
  std::vector<DATA_TYPE> input;
  input.resize(input_size);

  DATA_TYPE find_initial_value = -88;
  input[put_at_pos] = find_initial_value;
  DATA_TYPE find_image_value = convert_func(find_initial_value);
  auto compare_func = [&](auto const& item) { return (convert_func(item) == find_image_value); };
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

  if ( ! compare_func(input[put_at_pos]))
  {
    std::cout << "!=!=!=!=!=!=!=!=!=! ERROR (v2) @find_test: compare_func(input[put_at_pos]) returns false.";
    std::terminate();
  }

  auto view_in  = kwk::view{kwk::source = input.data(), kwk::of_size(d0)};

  int res_kwk_cpu, res_std, res_hand, res_eve;

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
  // b.set_iterations(1);
  b.run_function("std::execution::seq", fct_std);


  #if ENABLE_TBB
    int res_std_par, res_std_par_unseq, res_std_unseq;
    auto fct_std_par = [&]()
    {
      auto pos = std::find_if(std::execution::par, input.begin(), input.end(), compare_func);
      if (pos != input.end()) res_std_par = std::distance(input.begin(), pos);
      else                    res_std_par = -1;
      return res_std_par;
    };

    auto fct_std_par_unseq = [&]()
    {
      auto pos = std::find_if(std::execution::par_unseq, input.begin(), input.end(), compare_func);
      if (pos != input.end()) res_std_par_unseq = std::distance(input.begin(), pos);
      else                    res_std_par_unseq = -1;
      return res_std_par_unseq;
    };

    auto fct_std_unseq = [&]()
    {
      auto pos = std::find_if(std::execution::par_unseq, input.begin(), input.end(), compare_func);
      if (pos != input.end()) res_std_unseq = std::distance(input.begin(), pos);
      else                    res_std_unseq = -1;
      return res_std_unseq;
    };
    b.run_function("std::execution::unseq"    , fct_std_unseq);
    b.run_function("std::execution::par"      , fct_std_par);
    b.run_function("std::execution::par_unseq", fct_std_par_unseq);
    TTS_EQUAL(res_std_unseq     , res_std);
    TTS_EQUAL(res_std_par       , res_std);
    TTS_EQUAL(res_std_par_unseq , res_std);
  #endif

  auto fct_eve = [&]()
  {
    auto pos = kwk::find_if(kwk::simd, view_in, compare_func_eve);
    res_eve = kwk::utils::tools::pos_to_linear(view_in, pos);
    return res_eve;
  };
  b.run_function(kwk::bench::EVE_BACKEND_NAME, fct_eve);
  TTS_EQUAL(res_eve, res_std);

  b.run_function("Kiwaku on CPU", fct_kwk_cpu);
  TTS_EQUAL(res_kwk_cpu       , res_std);

  b.run_function("By hand on CPU", fct_hand);
  TTS_EQUAL(res_hand          , res_std);

  b.stop();
}



enum find_test_pos { first, middle, last };

void find_test_compute_bound(find_test_pos pos)
{
  if (::kwk::bench::enable_global)
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
                        , "find-if_compute-bound_" + pos_str + "_" + kwk::bench::EVE_COMPILER_FLAG + ".bench"
                        , convert_func
                        , convert_func_eve
                        , size
                        , put_at_pos
                        );
    std::cout << "\n\n";
  }
  else
  {
    TTS_EQUAL(true, true);
  }
}

TTS_CASE("Benchmark - find-if, compute-bound, last pos")    { find_test_compute_bound(find_test_pos::last);   };
TTS_CASE("Benchmark - find-if, compute-bound, middle pos")  { find_test_compute_bound(find_test_pos::middle); };
// TTS_CASE("Benchmark - find-if, compute-bound, first pos")   { find_test_compute_bound(find_test_pos::first);  };





void find_test_memory_bound(find_test_pos pos)
{
  if (::kwk::bench::enable_global)
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
  else
  {
    TTS_EQUAL(true, true);
  }
}

// sqrt(4 * cos * cos + sin * sin)

TTS_CASE("Benchmark - find-if, memory-bound, last pos")    { find_test_memory_bound(find_test_pos::last);   };
TTS_CASE("Benchmark - find-if, memory-bound, middle pos")  { find_test_memory_bound(find_test_pos::middle); };
// TTS_CASE("Benchmark - find-if, memory-bound, first pos")   { find_test_memory_bound(find_test_pos::first);  };

