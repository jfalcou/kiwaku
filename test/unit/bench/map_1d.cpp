//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/context/sycl/context.hpp>
#include <kwk/context/sycl/internal/sycl_tools.hpp>
#include <cstdlib>
#include <kwk/algorithm/algos/find.hpp>
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/reduce.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <numeric>
#include <optional>
#include "include/benchmark.hpp"
#include "include/utils/utils.hpp"
#include <cmath>
#include <execution> // don't forget the -ltbb compiler flag


template<typename DATA_TYPE>
void for_each_test( std::string const& bench_name
                  , std::string const& file_name
                  , auto map_func
                  , std::size_t const view_size
                  )
{
  const std::size_t d0 = view_size;
  const std::size_t data_size = d0;
  std::vector<DATA_TYPE> data;
  std::vector<DATA_TYPE> data_std; // remember result to check other values
  data.resize(data_size);
  data_std.resize(data_size);

  // FMA
  // auto map_func = [=](DATA_TYPE& d) { d = d * 8.7845f + 3.7645f); };

  auto reset_data = [&](std::vector<DATA_TYPE>& vect)
  {
    srand(9546312);
    DATA_TYPE min_value = 0;
    DATA_TYPE max_value = 8465124931;
    for (std::size_t i = 0; i < data_size; ++i)
      {
        vect[i] = kwk::bench::random_float<DATA_TYPE>(min_value, max_value);
      }
  };

  auto view_data  = kwk::view{kwk::source = data.data(), kwk::of_size(d0)};

  auto fct_kwk_cpu = [&]()
  {
    kwk::for_each(map_func, view_data);
    return view_data(view_size / 2);
  };

  auto fct_std = [&]()
  {
    std::for_each(data_std.begin(), data_std.end(), map_func);
    return data_std[view_size / 2];
  };

  // Don't forget the -ltbb compiler flag
  auto fct_std_par = [&]()
  {
    std::for_each(std::execution::par, data_std.begin(), data_std.end(), map_func);
    return data_std[view_size / 2];
  };

  // Don't forget the -ltbb compiler flag
  auto fct_std_par_unseq = [&]()
  {
    std::for_each(std::execution::par_unseq, data_std.begin(), data_std.end(), map_func);
    return data_std[view_size / 2];
  };

  auto fct_hand = [&]()
  {
    for (std::size_t i = 0; i < data_size; ++i)
    {
      map_func(data[i]);
    }
  return data[view_size / 2];
  };

  kwk::bench::cbench_t b;
  std::string absolute_path = ""; // will output to "kiwaku_build"

  b.start(absolute_path + kwk::bench::fprefix() + file_name, bench_name, "Processed elements, per second (higher is better)", view_size);
  // b.set_iterations(1);

  b.run_function("std::execution::seq", fct_std, [&]{ reset_data(data_std); }); // "std::for_each, single thread on CPU
  b.run_function("std::execution::par", fct_std_par, [&]{ reset_data(data_std); });
  b.run_function("std::execution::par_unseq", fct_std_par_unseq, [&]{ reset_data(data_std); });

  // Don't forget -fsycl-targets=nvptx64-nvidia-cuda
  bool has_gpu = kwk::sycl::has_gpu();

  auto sycl_bench = [&](auto&& context, DATA_TYPE& return_) -> DATA_TYPE
  {
    auto fct_kwk_sycl_generic = [&]()
    {
      kwk::for_each(context, map_func, view_data);
      return_ = view_data(view_size / 2);
      return return_;
    };
    b.run_function("Kiwaku SYCL on " + context.get_device_name(), fct_kwk_sycl_generic, [&]{ reset_data(data); });
    return return_;
  };

  // Execute SYCL benchmark on GPU and CPU
  if (has_gpu)
  {
    DATA_TYPE return_;
    // GPU
    sycl_bench(kwk::sycl::context{::sycl::gpu_selector_v}, return_);
    TTS_ALL_RELATIVE_EQUAL(data, data_std, 1);
    // CPU
    sycl_bench(kwk::sycl::context{::sycl::cpu_selector_v}, return_);
    TTS_ALL_RELATIVE_EQUAL(data, data_std, 1);
  }
  else // SYCL default context
  {
    DATA_TYPE return_;
    // CPU
    sycl_bench(kwk::sycl::default_context, return_);
    TTS_ALL_RELATIVE_EQUAL(data, data_std, 1);
  }

  b.run_function("Kiwaku on CPU", fct_kwk_cpu, [&]{ reset_data(data); });
  TTS_ALL_RELATIVE_EQUAL(data, data_std, 1);

  b.run_function("By hand on CPU", fct_hand, [&]{ reset_data(data); });
  TTS_ALL_RELATIVE_EQUAL(data, data_std, 1);
  b.stop();
}



TTS_CASE("Benchmark - for_each, compute-bound_trigo")
{
  if (::kwk::bench::enable_global)
  {
    using DATA_TYPE = float;

    auto map_func = [=](DATA_TYPE& item)
    {
      // TODO: test with multiplications and divisions
      // Modulo may imply thread divergence, which would impair GPU performance.
      item = static_cast<DATA_TYPE>(
        std::cos(
          std::sin(
            std::cos(
              std::sin(
                std::cos(
                  std::sin(
                    static_cast<float>(item * 11.f) * 7.f
                          ) * 3.8f
                        ) / 1.12f
                      ) * 3.17874f
                    ) / 8.7f
                  ) * 9.48f
                ) / 89.647681f
              ) * 1.8746221f;
    };

    [[maybe_unused]] std::size_t kio = 1024 / sizeof(DATA_TYPE);
    [[maybe_unused]] std::size_t mio = 1024 * kio;
    [[maybe_unused]] std::size_t gio = 1024 * mio;

    std::size_t size;
    std::string hname = sutils::get_host_name();
        if (hname == "parsys-legend")          { size =   1 * gio * kwk::bench::LEGEND_LOAD_FACTOR; } 
    else if (hname == "pata")                   { size =  64 * mio; }
    else if (hname == "chaton")                 { size =  64 * mio; }
    else if (hname == "sylvain-ThinkPad-T580")  { size =   8 * mio; }
    else if (hname == "lapierre")               { size =   8 * mio; }
    else                                        { size =   8 * mio; }
    

    sutils::printer_t::head("Benchmark - for_each, compute-bound_trigo", true);

    for_each_test<DATA_TYPE>("for_each compute-bound_trigo", "for_each_compute-bound_trigo.bench", map_func, size);
    std::cout << "\n\n";
  }
  else
  {
    TTS_EQUAL(true, true);
  }
};


// TTS_CASE("Benchmark - for_each, fma")
// {
//   if (::kwk::bench::enable_global)
//   {
//     using DATA_TYPE = float;

//     // FMA
//     auto map_func = [=](DATA_TYPE& d) { d = d * 8.7845f + 3.7645f; };

//     [[maybe_unused]] std::size_t kio = 1024 / sizeof(DATA_TYPE);
//     [[maybe_unused]] std::size_t mio = 1024 * kio;
//     [[maybe_unused]] std::size_t gio = 1024 * mio;

//     std::size_t size;
//     std::string hname = sutils::get_host_name();
//         if (hname == "parsys-legend")          { size =   6 * gio * kwk::bench::LEGEND_LOAD_FACTOR; } 
//     else if (hname == "pata")                   { size =  64 * mio; }
//     else if (hname == "chaton")                 { size =  64 * mio; }
//     else if (hname == "sylvain-ThinkPad-T580")  { size =   8 * mio; }
//     else if (hname == "lapierre")               { size =   8 * mio; }
//     else                                        { size =   8 * mio; }
    

//     sutils::printer_t::head("Benchmark - for_each, compute-bound_fma", true);

//     for_each_test<DATA_TYPE>("for_each compute-bound_fma", "for_each_compute-bound_fma.bench", map_func, size);
//     std::cout << "\n\n";
//   }
//   else
//   {
//     TTS_EQUAL(true, true);
//   }
// };

TTS_CASE("Benchmark - for_each, memory-bound")
{
  if (::kwk::bench::enable_global)
  {
    using DATA_TYPE = float;

    // FMA
    auto map_func = [=](DATA_TYPE& d) { d = d * 2.f; };

    [[maybe_unused]] std::size_t kio = 1024 / sizeof(DATA_TYPE);
    [[maybe_unused]] std::size_t mio = 1024 * kio;
    [[maybe_unused]] std::size_t gio = 1024 * mio;

    std::size_t size;
    std::string hname = sutils::get_host_name();
        if (hname == "parsys-legend")          { size =   6 * gio * kwk::bench::LEGEND_LOAD_FACTOR; } 
    else if (hname == "pata")                   { size =  64 * mio; }
    else if (hname == "chaton")                 { size =  64 * mio; }
    else if (hname == "sylvain-ThinkPad-T580")  { size =  64 * mio; }
    else if (hname == "lapierre")               { size =  64 * mio; }
    else                                        { size =  64 * mio; }
    

    sutils::printer_t::head("Benchmark - for_each, memory-bound", true);

    for_each_test<DATA_TYPE>("for_each memory-bound", "for_each_memory-bound.bench", map_func, size);
    std::cout << "\n\n";
  }
  else
  {
    TTS_EQUAL(true, true);
  }
};




