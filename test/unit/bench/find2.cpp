//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

#if KIWAKU_BENCH_BUILD

#include "include/benchmark.hpp"
#include "include/utils/utils.hpp"

#if KIWAKU_BENCH_KWK_SYCL_CPU || KIWAKU_BENCH_KWK_SYCL_GPU
  #include <kwk/context/sycl/context.hpp>
#endif

#if KIWAKU_BENCH_KWK_SIMD
  #include <kwk/context/eve/context.hpp>
#endif

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
              , ::kwk::bench::device_type_t device_type       // {cpu, gpu, cpu_and_gpu}
              , ::kwk::bench::bench_type_t bench_type         // {compute_CPU, compute_GPU, memory, unknown}
              , auto convert_func
              , auto convert_func_eve
              , auto convert_func_sycl
              , std::size_t const L2_length // number of elements contained in L2 cache (number of values, NOT size in bytes)
              , std::size_t const repetitions_over_array
              , std::size_t const put_at_pos
              , double clock_speed_CPU // expressed in GHz
              // , bool enable_gpu unused?
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

  bool bench_cpu = false;
  bool bench_gpu = false;
  switch (device_type)
  {
    case cpu:
      bench_cpu = true;
      bench_gpu = false;
      break;

    case gpu:
      bench_cpu = false;
      bench_gpu = true;
      break;

    case cpu_and_gpu:
      bench_cpu = true;
      bench_gpu = true;
      break;
  }

  std::vector<DATA_TYPE> input(L2_length);

  DATA_TYPE find_initial_value = -88;
  input[put_at_pos] = find_initial_value;
  DATA_TYPE find_image_value = convert_func(find_initial_value);
  auto compare_func = [=](auto const& item) { return (convert_func(item) == find_image_value); };
  auto compare_func_eve = [&](auto const& item) { return (convert_func_eve(item) == find_image_value); };

  #if KIWAKU_BENCH_KWK_SYCL_CPU || KIWAKU_BENCH_KWK_SYCL_GPU
    auto compare_func_sycl = [&](auto const& item) { return (convert_func_sycl(item) == find_image_value); };
  #endif

  if ( ! compare_func(input[put_at_pos]))
  {
    std::cout << "!=!=!=!=!=!=!=!=!=! ERROR (v1) @find_test: compare_func(input[put_at_pos]) returns false.";
    std::terminate();
  }

  // DATA_TYPE max_value = 8465124931.;

  // bool fast_init = true;


  if (KIWAKU_BENCH_FAST_INIT)
  {
    for (std::size_t i = 0; i < L2_length; ++i)
    {
      if (i == put_at_pos) continue;
      input[i] = 2;
    }
  }
  else
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

  std::size_t res_truth;
  bool truth_initialized = false;

  check_result = [&](auto const& value) -> bool
  {
    if (! KIWAKU_BENCH_ASSERT) return false;
    if (! truth_initialized)
    {
      res_truth = value;
      truth_initialized = true;
      return false;
    }
    return true; // must check results
  };



  // ====== hand-written sequential ======
  if (KIWAKU_BENCH_HAND && bench_cpu)
  {
    std::size_t res_hand;
    auto fct_hand = [&]() -> std::size_t
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
    if (check_result(res_hand)) TTS_EQUAL(res_kwk_cpu, res_truth);
  }

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


  // ====== Kiwaku CPU ======
  #if KIWAKU_BENCH_KWK_SEQ
    if (bench_cpu)
    {
      std::size_t res_kwk_cpu = bench_kiwaku(::kwk::cpu, "kwk CPU", compare_func);
      if (check_result(res_kwk_cpu)) TTS_EQUAL(res_kwk_cpu, res_truth);
    }
  #endif


  // ====== Kiwaku SIMD ======
  #if KIWAKU_BENCH_KWK_SIMD
    if (bench_cpu)
    {
      std::size_t res_kwk_simd = bench_kiwaku(::kwk::simd, "kwk simd", compare_func_eve);
      if (check_result(res_kwk_simd)) TTS_EQUAL(res_kwk_simd, res_truth);
    }
  #endif


  // ====== Generic std bench ======
  [[maybe_unused]] auto bench_std = [&](auto const& policy, std::string context_name)
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

  if (bench_cpu)
  {
    // ====== std sequential ======
    #if KIWAKU_BENCH_STD_SEQ
      std::size_t res_std_seq = bench_std(std::execution::seq, "seq");
      if (check_result(res_std_seq)) TTS_EQUAL(res_std_seq, res_truth);
    #endif


    // ====== std unsequenced ======
    #if KIWAKU_BENCH_STD_UNSEQ
      std::size_t res_std_unseq = bench_std(std::execution::unseq, "unseq");
      if (check_result(res_std_unseq)) TTS_EQUAL(res_std_unseq, res_truth);
    #endif


    // ====== std parallel ======
    #if KIWAKU_BENCH_STD_PAR
      // Don't forget the -ltbb compiler flag
      std::size_t res_std_par = bench_std(std::execution::par, "par");
      if (check_result(res_std_par)) TTS_EQUAL(res_std_par, res_truth);
    #endif


    // ====== std parallel unsequenced ======
    #if KIWAKU_BENCH_STD_PAR_UNSEQ
      // Don't forget the -ltbb compiler flag
      std::size_t res_std_par_unseq = bench_std(std::execution::par_unseq, "par_unseq");
      if (check_result(res_std_par_unseq)) TTS_EQUAL(res_std_par_unseq, res_truth);
    #endif
  }


  #if KIWAKU_BENCH_KWK_SYCL_CPU || KIWAKU_BENCH_KWK_SYCL_GPU
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
  #endif


  // ====== Kiwaku SYCL CPU ======
  #if KIWAKU_BENCH_KWK_SYCL_CPU
    if (bench_cpu)
    {
      auto ctx_cpu = kwk::sycl::context{::sycl::cpu_selector_v};
      std::size_t res_sycl_cpu = bench_kiwaku(ctx_cpu, "kwk SYCL " + ctx_cpu.get_device_name(), compare_func);
      if (check_result(res_sycl_cpu)) TTS_EQUAL(res_sycl_cpu, res_truth);
    }
  #endif


  // ====== Kiwaku SYCL GPU ======
  // Don't forget -fsycl-targets=nvptx64-nvidia-cuda,x86_64  (with x86_64 or spir64)
  #if KIWAKU_BENCH_KWK_SYCL_GPU
    if (bench_gpu)
    {
      bool has_gpu = kwk::sycl::has_gpu();
      TTS_EQUAL(has_gpu, true);
      if (has_gpu)
      {
        auto ctx_gpu = kwk::sycl::context{::sycl::gpu_selector_v};
        std::size_t res_sycl_gpu = bench_kiwaku(ctx_gpu, "kwk SYCL " + ctx_gpu.get_device_name(), compare_func_sycl);
        if (check_result(res_sycl_gpu)) TTS_EQUAL(res_sycl_gpu, res_truth);

        std::size_t res_sycl_gpu2 = bench_kiwaku_sycl_smart(ctx_gpu, "kwk SYCL persistent proxy " + ctx_gpu.get_device_name(), compare_func_sycl);
        if (check_result(res_sycl_gpu2)) TTS_EQUAL(res_sycl_gpu2, res_truth);
      }
      else
      {
        std::cout << "ERROR @ benchmark " << __FILE__ << ", line: " << __LINE__ << "\n";
        std::cout << "        unable to find a SYCL-compatible GPU on your system.\n";
        std::cout << "        KIWAKU_BENCH_KWK_SYCL_GPU is therefore temporarily disabled for this benchmark.\n";
      }
    }
  #endif

  b.stop();
}



enum find_test_pos { first, middle, last };



#if DESPAIR

enum bench_type_t {compute_CPU, compute_GPU, memory, unknown};
enum mem_type_t {RAM, L2, unknown_mem};
enum trigo_function_t {sycl_base, sycl_native, std_base};

void find_test_DESPAIR_bound( bench_type_t bench_type               // {compute_CPU, compute_GPU, memory, unknown}
                            , kwk::bench::mem_type_t mem_type       // {RAM, L2, unknown_mem}
                            , auto
                            , kwk::bench::trigo_function_t fct_type // {sycl_base, sycl_native, std_base}
                            , find_test_pos pos
                            , bool fit_inside_L2
                            )
{
  ::kwk::bench::get_eve_compiler_flag();

  using DATA_TYPE = float;

  std::size_t kio = 1024 / sizeof(DATA_TYPE);
  std::size_t mio = 1024 * kio;
  std::size_t gio = 1024 * mio;

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

  if (mem_type == kwk::bench::mem_type_t::RAM) L2_length = total_size;

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

  std::string mem_name = "unknown_memory_type ";
  if (mem_type == kwk::bench::mem_type_t::L2) mem_name = "L2 cache ";
  if (mem_type == kwk::bench::mem_type_t::RAM) mem_name = "RAM ";



  void find_test( std::string const& bench_name
                , std::string const& file_name
                , ::kwk::bench::device_type_t device_type       // {cpu, gpu, cpu_and_gpu}
                , ::kwk::bench::bench_type_t bench_type         // {compute_CPU, compute_GPU, memory, unknown}
                , auto convert_func
                , auto convert_func_eve
                , auto convert_func_sycl
                , std::size_t const L2_length // number of elements contained in L2 cache (number of values, NOT size in bytes)
                , std::size_t const repetitions_over_array
                , std::size_t const put_at_pos
                , double clock_speed_CPU // expressed in GHz
                // , bool enable_gpu unused?
                )

  // std::function<float()> convert_func, convert_func_eve, convert_func_sycl;


  if (bench_type == kwk::bench::bench_type_t::compute_CPU)
  {
    auto convert_func = [](auto const& in)
    {
      return (std::cos(in * 0.674651f) * std::cos(in * 0.921546f) + std::sin(in * 0.543217f) * std::sin(in * 0.754878f) + 2);
    };

    #if KIWAKU_BENCH_KWK_SIMD
      auto convert_func_eve = [](auto const& in)
      {
        return (eve::cos(in * 0.674651f) * eve::cos(in * 0.921546f) + eve::sin(in * 0.543217f) * eve::sin(in * 0.754878f) + 2);
      };
    #else
      auto convert_func_eve = convert_func;
    #endif

    #if KIWAKU_BENCH_KWK_SYCL_CPU
      auto convert_func_sycl = [](auto const& in)
      {
        return (eve::cos(in * 0.674651f) * eve::cos(in * 0.921546f) + eve::sin(in * 0.543217f) * eve::sin(in * 0.754878f) + 2);
      };


  }

  if (bench_type == kwk::bench::bench_type_t::compute_GPU)

  if (bench_type == kwk::bench::bench_type_t::memory)

  




  const std::size_t repeat_trigo_op = 512;
  INTERNAL_REPETITIONS = repeat_trigo_op * 2; // sin + cos = 2 operations

  const float A = 2.71828f;
  const float B = 3.14159f;
  // const float A = 0.674651f;
  // const float B = 1.543217f;


  std::string sycl_fname =  kwk::bench::trigo_function_to_fname(fct_type);

  std::string fname = "find-if_" + sycl_fname + "_" 
                    + pos_str + "_" 
                    + kwk::bench::EVE_COMPILER_FLAG 
                    + "_rep" + std::to_string(repetitions_over_array)
                    + "_L2-" + l2_str + ".bench";

  if (fct_type == kwk::bench::trigo_function_t::sycl_base)
  {
    auto convert_func = [A, B](auto in)
    {
      float x = in;
      for (std::size_t i = 0; i < repeat_trigo_op; ++i)
        x = ::sycl::cos(x * A) + ::sycl::sin(x * B);

      return x; //  (std::cos(in) + 2) * M_PI / 4 + 8 ; 
    };

    find_test<DATA_TYPE>("find-if " + sycl_fname + "-bound " + mem_name + pos_str
                        , fname
                        , convert_func
                        , convert_func_eve
                        , L2_length
                        , repetitions_over_array
                        , put_at_pos
                        , kwk::bench::bench_type_t::GPU_compute
                        , clock_speed_CPU
                        );
  }

  if (fct_type == kwk::bench::trigo_function_t::sycl_native)
  {
    auto convert_func = [A, B](auto in)
    {
      float x = in;
      for (std::size_t i = 0; i < repeat_trigo_op; ++i)
        x = ::sycl::native::cos(x * A) + ::sycl::native::sin(x * B);

      return x; //  (std::cos(in) + 2) * M_PI / 4 + 8 ; 
    };

    find_test<DATA_TYPE>("find-if " + sycl_fname + "-bound " + mem_name + pos_str
                        , fname
                        , convert_func
                        , convert_func_eve
                        , L2_length
                        , repetitions_over_array
                        , put_at_pos
                        , kwk::bench::bench_type_t::GPU_compute
                        , clock_speed_CPU
                        );
  }

  if (fct_type == kwk::bench::trigo_function_t::std_base)
  {
    auto convert_func = [A, B](auto in)
    {
      float x = in;
      for (std::size_t i = 0; i < repeat_trigo_op; ++i)
        x = ::std::cos(x * A) + ::std::sin(x * B);

      return x; //  (std::cos(in) + 2) * M_PI / 4 + 8 ; 
    };

    find_test<DATA_TYPE>("find-if " + sycl_fname + "-bound " + mem_name + pos_str
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
  }

  std::cout << "\n\n";
}

TTS_CASE("Benchmark - find-if, DESPAIR-bound, last pos")
{
  find_test_DESPAIR_bound(find_test_pos::last, false, true, kwk::bench::trigo_function_t::sycl_native);
  find_test_DESPAIR_bound(find_test_pos::last, false, true, kwk::bench::trigo_function_t::sycl_base);
  find_test_DESPAIR_bound(find_test_pos::last, false, true, kwk::bench::trigo_function_t::std_base);
};


#endif // #if ! DESPAIR


#if ! DESPAIR



void find_test_compute_bound(find_test_pos pos, bool fit_inside_L2, bool enable_gpu)
{
  ::kwk::bench::get_eve_compiler_flag();

  using DATA_TYPE = float;

  std::size_t kio = 1024 / sizeof(DATA_TYPE);
  std::size_t mio = 1024 * kio;
  std::size_t gio = 1024 * mio;

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

  std::size_t kio = 1024 / sizeof(DATA_TYPE);
  std::size_t mio = 1024 * kio;
  std::size_t gio = 1024 * mio;

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


#endif // KIWAKU_BENCH_BUILD