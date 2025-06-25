//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/context/eve/context.hpp>
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/reduce.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <numeric>
#include "../include/benchmark.hpp"
#include "../include/utils/utils.hpp"
// For parallel std execution, don't forget the -ltbb compiler flag
// #include <execution>



/// @brief 
/// @tparam DATA_TYPE must be a floating-point type
/// @param bench_name 
/// @param file_name 
/// @param func 
/// @param view_size  
template<typename DATA_TYPE>
void reduce_test(std::string const& bench_name, std::string const& file_name, auto func, DATA_TYPE func_id, std::size_t const view_size)
{

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

  DATA_TYPE res_kwk_cpu, res_std, res_hand, res_eve; // res_std_par, res_std_par_unseq, 

  auto fct_kwk_cpu = [&]() {
    res_kwk_cpu = kwk::reduce(view_in, func, input[0]);
    return res_kwk_cpu; };

  auto fct_std = [&]() {
    res_std = std::reduce(input.begin(), input.end(), input[0], func);
    return res_std; };

  // auto fct_std_par = [&]() {
  //   res_std_par = std::reduce(std::execution::par, input.begin(), input.end(), input[0], func);
  //   return res_std_par; };

  // auto fct_std_par_unseq = [&]() {
  //   res_std_par_unseq = std::reduce(std::execution::par_unseq, input.begin(), input.end(), input[0], func);
  //   return res_std_par_unseq; };

  auto fct_hand = [&]()
  {
    res_hand = input[0];
    for (std::size_t i = 1; i < input_size; ++i)
    {
      res_hand = func(res_hand, input[i]);
    }
    return res_hand;
  };

  kwk::bench::cbench_t b;
  std::string absolute_path = ""; // will output to "kiwaku_build"
  std::string final_fname = kwk::bench::fprefix() + file_name;

  b.start(absolute_path + final_fname, bench_name, "Processed elements, per second (higher is better)", view_size);
  b.run_function("std::execution::seq", fct_std);
  // b.run_function("std::execution::par", fct_std_par);
  // b.run_function("std::execution::par_unseq", fct_std_par_unseq);

  // //      reduce(kwk::simd, d,std::pair{eve::add, 0}, 10);
  // template<typename Op, typename Id, concepts::container In>
  // constexpr auto reduce([[maybe_unused]] kwk::eve::context const& ctx, In const& in, std::pair<Op, Id> R, auto init)

  auto fct_kwk_eve_generic = [&]()
  {
    res_eve = kwk::reduce(kwk::simd, view_in, std::pair{func, func_id}, input[0]);
    return res_eve;
  };
  b.run_function(kwk::bench::EVE_BACKEND_NAME, fct_kwk_eve_generic);

  b.run_function("Kiwaku on CPU", fct_kwk_cpu);
  b.run_function("By hand on CPU", fct_hand);
  b.stop();

  // TODO: TTS_RELATIVE_EQUAL

  // TTS_EQUAL(res_std_par, res_std);
  // TTS_RELATIVE_EQUAL(res_std_par      , res_std, ERROR_MAX_PERCENT);
  // TTS_RELATIVE_EQUAL(res_std_par_unseq, res_std, ERROR_MAX_PERCENT);
  TTS_RELATIVE_EQUAL(res_hand         , res_std, ERROR_MAX_PERCENT);
  TTS_RELATIVE_EQUAL(res_kwk_cpu      , res_std, ERROR_MAX_PERCENT);
  TTS_RELATIVE_EQUAL(res_eve          , res_std, ERROR_MAX_PERCENT);
}


TTS_CASE("Benchmark - reduce, memory-bound ")
{
  if (::kwk::bench::enable_global)
  {
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
    else if (hname == "chaton")                 { size = 128 * mio; }
    else if (hname == "sylvain-ThinkPad-T580")  { size =  32 * mio; }
    else if (hname == "lapierre")               { size =  32 * mio; }
    else                                        { size =   1 * gio; }

    reduce_test<DATA_TYPE>("Reduce memory-bound", "reduce_memory-bound_" + kwk::bench::EVE_COMPILER_FLAG + ".bench", func, func_id, size);
  }
  else
  {
    TTS_EQUAL(true, true);
  }
};
