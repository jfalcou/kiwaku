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
#include <kwk/context/sycl/internal/sycl_tools.hpp>
#include <cstdlib>
#include <kwk/algorithm/algos/find.hpp>
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/reduce.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <numeric>
#include <optional>
#include <cmath>
#include <execution> // don't forget the -ltbb compiler flag
#include <iostream>

#include <eve/module/math.hpp>

std::size_t INTERNAL_REPETITIONS = 1;
#define DESPAIR true


template<typename DATA_TYPE>
void find_test( std::string const& bench_name
              , std::string const& file_name
              , auto convert_func
              , auto convert_func_eve
              , std::size_t const L2_length // number of elements contained in L2 cache (number of values, NOT size in bytes)
              , std::size_t const repetitions_over_array
              , std::size_t const put_at_pos
              , ::kwk::bench::bench_type_t bench_type
              , double clock_speed_CPU // expressed in GHz
              , bool enable_gpu
              )
{

  // Total numer of element processed
  double total_number_of_elements_processed = L2_length * repetitions_over_array * INTERNAL_REPETITIONS;
  // Potentiellement aussi plutôt put_at_pos * repetitions_over_array
  // Même si c'est une métrique un peu foireuse étant donné que SYCL regarde tous les éléments
  // A voir.
  double bandwidth_per_element_read   = sizeof(DATA_TYPE);
  double bandwidth_per_element_write  = 0;
  double bandwidth_per_element_in_bytes = bandwidth_per_element_read + bandwidth_per_element_write;

  constexpr bool check = true;

  std::vector<DATA_TYPE> input(L2_length);

  DATA_TYPE find_initial_value = -88;
  input[put_at_pos] = find_initial_value;
  DATA_TYPE find_image_value = convert_func(find_initial_value);
  auto compare_func = [=](auto const& item) { return (convert_func(item) == find_image_value); };
  [[maybe_unused]] auto compare_func_eve = [&](auto const& item) { return (convert_func_eve(item) == find_image_value); };

  if ( ! compare_func(input[put_at_pos]))
  {
    std::cout << "!=!=!=!=!=!=!=!=!=! ERROR (v1) @find_test: compare_func(input[put_at_pos]) returns false.";
    std::terminate();
  }

  // DATA_TYPE max_value = 8465124931.;

  bool fast_init = true;


  if ( ! fast_init)
  {
    srand(9546312);
    DATA_TYPE min_value = 0.;
    DATA_TYPE max_value = M_PI * 2;

    for (std::size_t i = 0; i < L2_length; ++i)
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
  }
  else
  {
    for (std::size_t i = 0; i < L2_length; ++i)
    {
      if (i == put_at_pos) continue;
      input[i] = 2;
    }
  }

  if ( ! compare_func(input[put_at_pos]))
  {
    std::cout << "!=!=!=!=!=!=!=!=!=! ERROR (v2) @find_test: compare_func(input[put_at_pos]) returns false.";
    std::terminate();
  }

  auto view_in  = kwk::view{kwk::source = input.data(), kwk::of_size(L2_length)};

  std::string y_axis_title = "UNKNOWN AXIS TYPE";
  if (bench_type == kwk::bench::bench_type_t::compute) y_axis_title = "Cycles per element";
  if (bench_type == kwk::bench::bench_type_t::memory)  y_axis_title = "Throughput (GB/s)";

  kwk::bench::cbench_t b;
  b.start ( kwk::bench::fprefix() + file_name
          , bench_name
          , y_axis_title
          , total_number_of_elements_processed
          , bench_type
          );
  // b.set_iterations(1);

  [[maybe_unused]] std::size_t res_truth;

  #if ! DESPAIR
  // ====== hand-written sequential ======
  [[maybe_unused]] std::size_t res_hand;
  [[maybe_unused]] auto fct_hand = [&]() -> std::size_t
  {
    res_hand = 0;
    for (std::size_t r = 0; r < repetitions_over_array; ++r)
    {
      for (std::size_t i = 0; i < L2_length; ++i)
      {
        if (compare_func(input[i]))
        {
          res_hand += i;
          break;
        }
      }
    }
  return res_hand;
  };
  b.run_ext2( "hand CPU"
            , fct_hand
            , []{} // no reset function
            , total_number_of_elements_processed
            , bandwidth_per_element_in_bytes
            , bench_type
            , clock_speed_CPU
            , ::kwk::bench::device_type_t::cpu
            );
  res_truth = res_hand;
  #endif // #if ! DESPAIR

  // ====== Generic Kiwaku bench ======
  [[maybe_unused]] auto bench_kiwaku = [&](auto&& context, std::string context_name, auto compare_func_)
  {
    std::size_t ret;
    auto fct_kwk = [&]() -> std::size_t
    {
      ret = 0;
      for (std::size_t r = 0; r < repetitions_over_array; ++r)
      {
        auto pos = kwk::find_if(context, view_in, compare_func_);
        ret += kwk::utils::tools::pos_to_linear(view_in, pos);
      }
      return ret;
    };
    b.run_ext2( context_name
              , fct_kwk
              , []{} // no reset function
              , total_number_of_elements_processed
              , bandwidth_per_element_in_bytes
              , bench_type
              , clock_speed_CPU
              , ::kwk::bench::device_type_t::cpu
              );
    return ret;
  };

  auto bench_kiwaku_sycl_smart = [&](auto&& context, std::string context_name, auto compare_func_)
  {
    auto in_proxy = context.in(view_in);
    std::size_t ret;
    auto fct_kwk = [&]() -> std::size_t
    {
      ret = 0;
      for (std::size_t r = 0; r < repetitions_over_array; ++r)
      {
        auto pos = kwk::find_if_proxy(context, view_in, in_proxy, compare_func_);
        ret += kwk::utils::tools::pos_to_linear(view_in, pos);
      }
      return ret;
    };
    b.run_ext2( context_name
              , fct_kwk
              , []{} // no reset function
              , total_number_of_elements_processed
              , bandwidth_per_element_in_bytes
              , bench_type
              , clock_speed_CPU
              , ::kwk::bench::device_type_t::cpu
              );
    return ret;
  };


  #if ! DESPAIR
    // ====== Kiwaku CPU ======
    std::size_t res_kwk_cpu = bench_kiwaku(::kwk::cpu, "kwk CPU", compare_func);
    if constexpr(check) TTS_EQUAL(res_kwk_cpu, res_truth);


    // ====== Kiwaku SIMD ======
    #if KIWAKU_BENCH_EVE
      std::size_t res_kwk_simd = bench_kiwaku(::kwk::simd, "kwk simd", compare_func_eve);
      if constexpr(check) TTS_EQUAL(res_kwk_simd, res_truth);
    #endif


    // ====== Generic std bench ======
    auto bench_std = [&](auto const& policy, std::string context_name)
    {
      std::size_t ret;
      auto fct_std = [&]() -> std::size_t
      {
        ret = 0;
        for (std::size_t r = 0; r < repetitions_over_array; ++r)
        {
          auto pos = std::find_if(policy, input.begin(), input.end(), compare_func);
          if (pos != input.end()) ret += pos - input.begin(); //std::distance(input.begin(), pos);
          else                    ret += 0;
        }
        return ret;
      };
      b.run_ext2( context_name
                , fct_std
                , []{} // no reset function
                , total_number_of_elements_processed
                , bandwidth_per_element_in_bytes
                , bench_type
                , clock_speed_CPU
                , ::kwk::bench::device_type_t::cpu
                );
      return ret;
    };


    // ====== std sequential ======
    std::size_t res_std_seq = bench_std(std::execution::seq, "seq");
    if constexpr(check) TTS_EQUAL(res_std_seq, res_truth);


    // ====== std unsequenced ======
    std::size_t res_std_unseq = bench_std(std::execution::unseq, "unseq");
    if constexpr(check) TTS_EQUAL(res_std_unseq, res_truth);


    // Don't forget the -ltbb compiler flag
    #if ENABLE_TBB && KIWAKU_BENCH_MTHREAD

      // ====== std parallel ======
      std::size_t res_std_par = bench_std(std::execution::par, "par");
      if constexpr(check) TTS_EQUAL(res_std_par, res_truth);

      // ====== std parallel unsequenced ======
      std::size_t res_std_par_unseq = bench_std(std::execution::par_unseq, "par_unseq");
      if constexpr(check) TTS_EQUAL(res_std_par_unseq, res_truth);

    #endif // ENABLE_TBB && KIWAKU_BENCH_MTHREAD

  #endif // #if ! DESPAIR

  // ====== SYCL ======
  #if KIWAKU_BENCH_SYCL
    // Don't forget -fsycl-targets=nvptx64-nvidia-cuda,x86_64  (with x86_64 or spir64)
    bool has_gpu = kwk::sycl::has_gpu();

    if (has_gpu)
    {
      #if KIWAKU_BENCH_MTHREAD
        // ====== Kiwaku SYCL CPU ======
        auto ctx_cpu = kwk::sycl::context{::sycl::cpu_selector_v};
        std::size_t res_sycl_cpu = bench_kiwaku(ctx_cpu, "kwk SYCL " + ctx_cpu.get_device_name(), compare_func);
        if constexpr(check) TTS_EQUAL(res_sycl_cpu, res_truth);
      #endif

      if (enable_gpu)
      {
        // ====== Kiwaku SYCL GPU ======
        auto ctx_gpu = kwk::sycl::context{::sycl::gpu_selector_v};
        std::size_t res_sycl_gpu = bench_kiwaku(ctx_gpu, "kwk SYCL " + ctx_gpu.get_device_name(), compare_func);
        if constexpr(check) TTS_EQUAL(res_sycl_gpu, res_truth);

        std::size_t res_sycl_gpu2 = bench_kiwaku_sycl_smart(ctx_gpu, "kwk SYCL persistent proxy " + ctx_gpu.get_device_name(), compare_func);
        if constexpr(check) TTS_EQUAL(res_sycl_gpu2, res_truth);
      }
      
    }
    else // SYCL default context
    {
      #if KIWAKU_BENCH_MTHREAD
        // ====== Kiwaku SYCL CPU ======
        auto& ctx_cpu = kwk::sycl::default_context;
        std::size_t res_sycl_cpu = bench_kiwaku(ctx_cpu, "kwk SYCL " + ctx_cpu.get_device_name(), compare_func);
        if constexpr(check) TTS_EQUAL(res_sycl_cpu, res_truth);
      #endif
    }
  #endif

  b.stop();
}



enum find_test_pos { first, middle, last };



#if DESPAIR

void find_test_DESPAIR_bound(find_test_pos pos, bool fit_inside_L2, bool enable_gpu)
{
  ::kwk::bench::get_eve_compiler_flag();

  using DATA_TYPE = float;

  [[maybe_unused]] std::size_t kio = 1024 / sizeof(DATA_TYPE);
  [[maybe_unused]] std::size_t mio = 1024 * kio;
  [[maybe_unused]] std::size_t gio = 1024 * mio;

  std::size_t total_size, L2_length;
  double clock_speed_CPU = 0;
  std::string hname = sutils::get_host_name();

  if (hname == "parsys-legend")          
  {
    total_size = 1 * gio;
    // total_size = 1 * gio;
    L2_length = 256 * kio;
    clock_speed_CPU = 4.7;
  }

  if (hname == "chaton")          
  {
    total_size = 64 * mio;
    L2_length = 128 * kio;
    clock_speed_CPU = 3.6;
  } 

  if ( ! fit_inside_L2) L2_length = total_size;



  auto convert_func_eve = [](auto const& in)
  {
    return (eve::cos(in * 0.67465f) * eve::cos(in * 0.921546f) + eve::sin(in * 0.543217f) * eve::sin(in * 0.754878f) + 2);
  };

  INTERNAL_REPETITIONS = 512 * 2;

  const float A = 2.71828f;
  const float B = 3.14159f;
  // const float A = 0.674651f;
  // const float B = 1.543217f;
  auto convert_func = [A, B](auto in)
  {
    float x = in;
    for (std::size_t i = 0; i < 512; ++i)
      x = ::sycl::cos(x * A) + ::sycl::sin(x * B);

    return x; //  (std::cos(in) + 2) * M_PI / 4 + 8 ; 
  };


  std::string pos_str = "";
  std::size_t put_at_pos = 0;
  switch (pos)
  {
    case first:   pos_str = "pos(first)";  put_at_pos = 0;        break;
    case middle:  pos_str = "pos(middle)"; put_at_pos = L2_length / 2; break;
    case last:    pos_str = "pos(last)";   put_at_pos = L2_length - 1; break;
    default: break;
  }

  sutils::printer_t::head("Benchmark - find-if, compute-bound, " + pos_str, true);

  std::size_t repetitions_over_array = total_size / L2_length;
  std::string l2_str = std::to_string(L2_length / kio);

  std::string mem_name = "";
  if (fit_inside_L2) mem_name = "L2 cache ";
  else               mem_name = "RAM ";

  std::string fname = "find-if_DESPAIR_NORMAL_" 
                    + pos_str + "_" 
                    + kwk::bench::EVE_COMPILER_FLAG 
                    + "_rep" + std::to_string(repetitions_over_array)
                    + "_L2-" + l2_str + ".bench";

  find_test<DATA_TYPE>("find-if DESPAIR_NORMAL-bound " + mem_name + pos_str
                      , fname
                      , convert_func
                      , convert_func_eve
                      , L2_length
                      , repetitions_over_array
                      , put_at_pos
                      , kwk::bench::bench_type_t::GPU_compute
                      , clock_speed_CPU
                      , enable_gpu
                      );


  std::cout << "\n\n";
}

TTS_CASE("Benchmark - find-if, DESPAIR-bound, last pos")    { find_test_DESPAIR_bound(find_test_pos::last, false, true);   };


#endif // #if ! DESPAIR


#if ! DESPAIR



void find_test_compute_bound(find_test_pos pos, bool fit_inside_L2, bool enable_gpu)
{
  ::kwk::bench::get_eve_compiler_flag();

  using DATA_TYPE = float;

  [[maybe_unused]] std::size_t kio = 1024 / sizeof(DATA_TYPE);
  [[maybe_unused]] std::size_t mio = 1024 * kio;
  [[maybe_unused]] std::size_t gio = 1024 * mio;

  std::size_t total_size, L2_length;
  double clock_speed_CPU = 0;
  std::string hname = sutils::get_host_name();

  if (hname == "parsys-legend")          
  {
    total_size = 1 * gio;
    // total_size = 1 * gio;
    L2_length = 256 * kio;
    clock_speed_CPU = 4.7;
  }

  if (hname == "chaton")          
  {
    total_size = 64 * mio;
    L2_length = 128 * kio;
    clock_speed_CPU = 3.6;
  } 

  if ( ! fit_inside_L2) L2_length = total_size;



  auto convert_func_eve = [](auto const& in)
  {
    return (eve::cos(in * 0.67465f) * eve::cos(in * 0.921546f) + eve::sin(in * 0.543217f) * eve::sin(in * 0.754878f) + 2);
  };


  auto convert_func = [](auto const& in)
  {
    return (std::cos(in * 0.67465f) * std::cos(in * 0.921546f) + std::sin(in * 0.543217f) * std::sin(in * 0.754878f) + 2);
  };

  // auto convert_func_eve = [](auto const& item)
  // {
  //   // répétitions ici pour augmenter l'intensité ?
  //   return
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
  //   // ::eve::sin(item) * static_cast<DATA_TYPE>(11.)
  // };


  // auto convert_func = [](auto const& item)
  // {
  //   return
  //   ::std::cos(
  //     ::std::sin(
  //       ::std::cos(
  //         ::std::sin(
  //           ::std::cos(
  //             ::std::sin(
  //               item * static_cast<DATA_TYPE>(1.425)
  //             ) * static_cast<DATA_TYPE>(1.458)
  //           ) / static_cast<DATA_TYPE>(1.578)
  //         ) * static_cast<DATA_TYPE>(1.98794)
  //       ) / static_cast<DATA_TYPE>(1.578)
  //     ) * static_cast<DATA_TYPE>(1.98794)
  //   ) / static_cast<DATA_TYPE>(1.87961);
  //   // ::eve::sin(item) * static_cast<DATA_TYPE>(11.)
  // };
  

  std::string pos_str = "";
  std::size_t put_at_pos = 0;
  switch (pos)
  {
    case first:   pos_str = "pos(first)";  put_at_pos = 0;        break;
    case middle:  pos_str = "pos(middle)"; put_at_pos = L2_length / 2; break;
    case last:    pos_str = "pos(last)";   put_at_pos = L2_length - 1; break;
    default: break;
  }

  sutils::printer_t::head("Benchmark - find-if, compute-bound, " + pos_str, true);

  std::size_t repetitions_over_array = total_size / L2_length;
  std::string l2_str = std::to_string(L2_length / kio);

  std::string mem_name = "";
  if (fit_inside_L2) mem_name = "L2 cache ";
  else               mem_name = "RAM ";

  std::string fname = "find-if_compute_" 
                    + pos_str + "_" 
                    + kwk::bench::EVE_COMPILER_FLAG 
                    + "_rep" + std::to_string(repetitions_over_array)
                    + "_L2-" + l2_str + ".bench";

  find_test<DATA_TYPE>("find-if compute-bound " + mem_name + pos_str
                      , fname
                      , convert_func
                      , convert_func_eve
                      , L2_length
                      , repetitions_over_array
                      , put_at_pos
                      , kwk::bench::bench_type_t::compute
                      , clock_speed_CPU
                      , enable_gpu
                      );


  std::cout << "\n\n";
}



TTS_CASE("Benchmark - find-if, compute-bound, last pos")    { find_test_compute_bound(find_test_pos::last, false, true);   };
TTS_CASE("Benchmark - find-if, compute-bound, last pos")    { find_test_compute_bound(find_test_pos::last, true, true);   };

TTS_CASE("Benchmark - find-if, compute-bound, middle pos")    { find_test_compute_bound(find_test_pos::middle, false, true);   };
TTS_CASE("Benchmark - find-if, compute-bound, middle pos")    { find_test_compute_bound(find_test_pos::middle, true, true);   };

// TTS_CASE("Benchmark - find-if, compute-bound, middle pos")  { find_test_compute_bound(find_test_pos::middle); };






void find_test_memory_bound(find_test_pos pos, bool fit_inside_L2, bool enable_gpu)
{
  ::kwk::bench::get_eve_compiler_flag();

  using DATA_TYPE = float;

  [[maybe_unused]] std::size_t kio = 1024 / sizeof(DATA_TYPE);
  [[maybe_unused]] std::size_t mio = 1024 * kio;
  [[maybe_unused]] std::size_t gio = 1024 * mio;

  std::size_t total_size, L2_length;
  double clock_speed_CPU = 0;
  std::string hname = sutils::get_host_name();

  if (hname == "parsys-legend")          
  {
    // total_size = 6 * gio;
    total_size = 6 * gio;
    L2_length = 256 * kio;
    clock_speed_CPU = 4.7;
  }

  if (hname == "chaton")          
  {
    total_size = 256 * mio;
    L2_length = 128 * kio;
    clock_speed_CPU = 3.6;
  } 

  if ( ! fit_inside_L2) L2_length = total_size;

  std::string pos_str = "";
  std::size_t put_at_pos = 0;
  switch (pos)
  {
    case first:   pos_str = "pos(first)";  put_at_pos = 0;        break;
    case middle:  pos_str = "pos(middle)"; put_at_pos = L2_length / 2; break;
    case last:    pos_str = "pos(last)";   put_at_pos = L2_length - 1; break;
    default: break;
  }

  sutils::printer_t::head("Benchmark - find-if, memory-bound, " + pos_str, true);

  auto convert_func = [=](auto const& item) { return item; };
  // auto convert_func = [=](auto const& item) { return ::std::cos(item) * static_cast<DATA_TYPE>(1.45); };
  // auto convert_func_eve = [=](auto const& item) { return ::eve::cos(item) * static_cast<DATA_TYPE>(1.45); };

  std::string mem_name = "";
  if (fit_inside_L2) mem_name = "L2 cache ";
  else               mem_name = "RAM ";

  std::size_t repetitions_over_array = total_size / L2_length;
  std::string l2_str = std::to_string(L2_length / kio);

  std::string fname = "find-if_memory_" 
                    + pos_str + "_" 
                    + kwk::bench::EVE_COMPILER_FLAG 
                    + "_rep" + std::to_string(repetitions_over_array)
                    + "_L2-" + l2_str + ".bench";

  find_test<DATA_TYPE>("find-if memory-bound " + mem_name + pos_str
                      , fname
                      , convert_func
                      , convert_func
                      , L2_length
                      , repetitions_over_array
                      , put_at_pos
                      , kwk::bench::bench_type_t::memory
                      , clock_speed_CPU
                      , enable_gpu
                      );
  std::cout << "\n\n";
}

// sqrt(4 * cos * cos + sin * sin)

TTS_CASE("Benchmark - find-if, memory-bound, last pos")    { find_test_memory_bound(find_test_pos::last, false, true);   };
TTS_CASE("Benchmark - find-if, memory-bound, last pos")    { find_test_memory_bound(find_test_pos::last, true, false);   };

TTS_CASE("Benchmark - find-if, memory-bound, middle pos")    { find_test_memory_bound(find_test_pos::middle, false, true);   };
TTS_CASE("Benchmark - find-if, memory-bound, middle pos")    { find_test_memory_bound(find_test_pos::middle, true, false);   };

// TTS_CASE("Benchmark - find-if, memory-bound, middle pos")  { find_test_memory_bound(find_test_pos::middle); };

#endif // #if ! DESPAIR


#endif // KIWAKU_BUILD_BENCH