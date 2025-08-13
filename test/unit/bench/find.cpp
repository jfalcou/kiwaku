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


template<typename DATA_TYPE>
void find_test( std::string const& bench_name
              , std::string const& file_name
              , kwk::bench::device_type_t const device_type       // {cpu, gpu, cpu_and_gpu}
              , kwk::bench::bench_type_t const bench_type         // {compute_CPU, compute_GPU, memory, unknown}
              , auto convert_func
              , auto convert_func_eve
              , auto convert_func_sycl_base // TODO
              , auto convert_func_sycl_native // TODO
              , std::size_t const array_length // number of elements contained in L2 cache (number of values, NOT size in bytes)
              , std::size_t const repetitions_over_array
              , std::size_t const put_at_pos
              , double clock_speed_CPU // expressed in GHz
              // , bool enable_gpu unused?
              )
{

  // Total numer of element processed
  double total_number_of_elements_processed = array_length * repetitions_over_array * INTERNAL_REPETITIONS;
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
    case kwk::bench::device_type_t::cpu:
      bench_cpu = true;
      bench_gpu = false;
      break;

    case kwk::bench::device_type_t::gpu:
      bench_cpu = false;
      bench_gpu = true;
      break;

    case kwk::bench::device_type_t::cpu_and_gpu:
      bench_cpu = true;
      bench_gpu = true;
      break;
  }

  std::vector<DATA_TYPE> input(array_length);

  DATA_TYPE find_initial_value = -88;
  input[put_at_pos] = find_initial_value;
  DATA_TYPE find_image_value = convert_func(find_initial_value);
  auto compare_func = [=](auto item) { return (convert_func(item) == find_image_value); };

  #if KIWAKU_BENCH_KWK_SYCL_CPU || KIWAKU_BENCH_KWK_SYCL_GPU
    
    
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
    for (std::size_t i = 0; i < array_length; ++i)
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

    for (std::size_t i = 0; i < array_length; ++i)
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

  auto view_in  = kwk::view{kwk::source = input.data(), kwk::of_size(array_length)};

  std::string y_axis_title = "UNKNOWN AXIS TYPE";
  if (bench_type == kwk::bench::bench_type_t::compute_CPU) y_axis_title = "Cycles per element";
  if (bench_type == kwk::bench::bench_type_t::compute_GPU) y_axis_title = "Elapsed (ms)";
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

  auto check_result = [&](auto const& value) -> bool
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
        for (std::size_t i = 0; i < array_length; ++i)
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
              , kwk::bench::device_type_t::cpu
              );
    if (check_result(res_hand)) TTS_EQUAL(res_hand, res_truth);
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
              , kwk::bench::device_type_t::cpu
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
      auto compare_func_eve = [&](auto item) { return (convert_func_eve(item) == find_image_value); };
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
              , kwk::bench::device_type_t::cpu
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
                , kwk::bench::device_type_t::cpu
                );
      return ret;
    };
  #endif


  // ====== Kiwaku SYCL CPU ======
  #if KIWAKU_BENCH_KWK_SYCL_CPU
    if (bench_cpu)
    {
      auto ctx_cpu = kwk::sycl::context{::sycl::cpu_selector_v};

      if (kwk::bench::is_compute_bench(bench_type))
      {
        std::size_t res_sycl_cpu = bench_kiwaku(ctx_cpu, "kwk SYCL std:: " + ctx_cpu.get_device_name(), compare_func);
        if (check_result(res_sycl_cpu)) TTS_EQUAL(res_sycl_cpu, res_truth);
        
        auto compare_func_sycl_base = [=](auto item) { return (convert_func_sycl_base(item) == find_image_value); };
        std::size_t res_sycl_cpu2   = bench_kiwaku(ctx_cpu, "kwk SYCL sycl:: " + ctx_cpu.get_device_name(), compare_func_sycl_base);
        if (check_result(res_sycl_cpu2)) TTS_EQUAL(res_sycl_cpu, res_truth);

        auto compare_func_sycl_native = [=](auto item) { return (convert_func_sycl_native(item) == find_image_value); };
        std::size_t res_sycl_cpu3     = bench_kiwaku(ctx_cpu, "kwk SYCL sycl::native:: " + ctx_cpu.get_device_name(), compare_func_sycl_native);
        if (check_result(res_sycl_cpu3)) TTS_EQUAL(res_sycl_cpu, res_truth);
      }
      else
      {
        std::size_t res_sycl_cpu = bench_kiwaku(ctx_cpu, "kwk SYCL " + ctx_cpu.get_device_name(), compare_func);
        if (check_result(res_sycl_cpu)) TTS_EQUAL(res_sycl_cpu, res_truth);
      }
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

        if (kwk::bench::is_compute_bench(bench_type))
        {
          auto compare_func_sycl_base   = [=](auto item) { return (convert_func_sycl_base(item) == find_image_value); };
          auto compare_func_sycl_native = [=](auto item) { return (convert_func_sycl_native(item) == find_image_value); };

          std::size_t res_sycl_gpu10 = bench_kiwaku(ctx_gpu, "kwk SYCL std:: " + ctx_gpu.get_device_name(), compare_func);
          if (check_result(res_sycl_gpu10)) TTS_EQUAL(res_sycl_gpu10, res_truth);

          std::size_t res_sycl_gpu11 = bench_kiwaku(ctx_gpu, "kwk SYCL sycl:: " + ctx_gpu.get_device_name(), compare_func_sycl_base);
          if (check_result(res_sycl_gpu11)) TTS_EQUAL(res_sycl_gpu11, res_truth);

          std::size_t res_sycl_gpu12 = bench_kiwaku(ctx_gpu, "kwk SYCL sycl::native:: " + ctx_gpu.get_device_name(), compare_func_sycl_native);
          if (check_result(res_sycl_gpu12)) TTS_EQUAL(res_sycl_gpu12, res_truth);

          std::size_t res_sycl_gpu20 = bench_kiwaku_sycl_smart(ctx_gpu, "kwk SYCL std:: persistent proxy " + ctx_gpu.get_device_name(), compare_func);
          if (check_result(res_sycl_gpu20)) TTS_EQUAL(res_sycl_gpu20, res_truth);

          std::size_t res_sycl_gpu21 = bench_kiwaku_sycl_smart(ctx_gpu, "kwk SYCL sycl:: persistent proxy " + ctx_gpu.get_device_name(), compare_func_sycl_base);
          if (check_result(res_sycl_gpu21)) TTS_EQUAL(res_sycl_gpu21, res_truth);

          std::size_t res_sycl_gpu22 = bench_kiwaku_sycl_smart(ctx_gpu, "kwk SYCL sycl::native persistent proxy " + ctx_gpu.get_device_name(), compare_func_sycl_native);
          if (check_result(res_sycl_gpu22)) TTS_EQUAL(res_sycl_gpu22, res_truth);
        }
        else // memory-bound benchmark
        {
          std::size_t res_sycl_gpu10 = bench_kiwaku(ctx_gpu, "kwk SYCL " + ctx_gpu.get_device_name(), compare_func);
          if (check_result(res_sycl_gpu10)) TTS_EQUAL(res_sycl_gpu10, res_truth);

          std::size_t res_sycl_gpu20 = bench_kiwaku_sycl_smart(ctx_gpu, "kwk SYCL persistent proxy " + ctx_gpu.get_device_name(), compare_func);
          if (check_result(res_sycl_gpu20)) TTS_EQUAL(res_sycl_gpu20, res_truth);
        }
      }
      else
      {
        std::cout << std::endl;
        std::cout << "ERROR @ benchmark " << __FILE__ << ", line: " << __LINE__ << "\n";
        std::cout << "        unable to find a SYCL-compatible GPU on your system.\n";
        std::cout << "        KIWAKU_BENCH_KWK_SYCL_GPU is therefore temporarily disabled for this benchmark.\n";
      }
    }
  #endif

  b.stop();
}



enum find_test_pos { first, middle, last };


void find_test_generic( kwk::bench::bench_type_t bench_type   // {compute_CPU, compute_GPU, memory, unknown}
                      , kwk::bench::device_type_t device_type // {cpu, gpu, cpu_and_gpu}
                      , kwk::bench::mem_type_t mem_type       // {RAM, L2, unknown_mem}
                      // , kwk::bench::trigo_function_t fct_type // {sycl_base, sycl_native, std_base}
                      , auto convert_func
                      , auto convert_func_eve
                      , auto convert_func_sycl_base
                      , auto convert_func_sycl_native
                      , find_test_pos pos
                      )
{
  kwk::bench::get_eve_compiler_flag();

  using DATA_TYPE = float;

  std::size_t kio = 1024 / sizeof(DATA_TYPE);
  std::size_t mio = 1024 * kio;
  // std::size_t gio = 1024 * mio;

  std::size_t total_length;

  if (is_compute_bench(bench_type)) total_length = KIWAKU_BENCH_COMPUTE_DATA_SIZE_MB * mio;
  else                              total_length = KIWAKU_BENCH_MEMORY_DATA_SIZE_MB * mio;

  // Take half the real L2 size to make sure data entirely fits in L2 cache
  std::size_t const  L2_length = KIWAKU_BENCH_L2_SIZE_KB * kio / 2;
  double const clock_speed_CPU = KIWAKU_BENCH_CPU_CLOCK_SPEED_GHz;



  
  // KIWAKU_BENCH_CPU_CLOCK_SPEED_GHz="4.1"
  // KIWAKU_BENCH_MEMORY_DATA_SIZE_MB="4096"
  // KIWAKU_BENCH_COMPUTE_DATA_SIZE_MB="64"
  // KIWAKU_BENCH_L2_SIZE_KB="256"
  
  // std::string hname = sutils::get_host_name();
  // if (hname == "parsys-legend")          
  // {
  //   total_length = 1 * gio;
  //   // total_length = 1 * gio;
  //   L2_length = 256 * kio;
  //   clock_speed_CPU = 4.7;
  // }
  // if (hname == "chaton")          
  // {
  //   total_length = 64 * mio;
  //   L2_length = 128 * kio;
  //   clock_speed_CPU = 3.6;
  // }

  std::size_t array_length = 0;
  if (mem_type == kwk::bench::mem_type_t::RAM)  array_length = total_length;
  if (mem_type == kwk::bench::mem_type_t::L2)   array_length = L2_length;

  std::string pos_str = "";
  std::size_t put_at_pos = 0;
  switch (pos)
  {
    case first:   pos_str = "pos(first)";  put_at_pos = 0;        break;
    case middle:  pos_str = "pos(middle)"; put_at_pos = array_length / 2; break;
    case last:    pos_str = "pos(last)";   put_at_pos = array_length - 1; break;
    default: break;
  }

  sutils::printer_t::head ( "Benchmark - find_if, "
                            + kwk::bench::bench_type_to_str(bench_type) 
                            + ", " + pos_str
                          , true
                          );

  std::size_t repetitions_over_array = total_length / array_length;
  std::string array_len_str = std::to_string(array_length / kio);

  std::string mem_name = "unknown_memory_type ";
  if (mem_type == kwk::bench::mem_type_t::L2)   mem_name = "L2 cache";
  if (mem_type == kwk::bench::mem_type_t::RAM)  mem_name = "RAM";

  std::string fname, bench_name;

  fname = "find_if_" 
        + pos_str + "_" 
        + kwk::bench::EVE_COMPILER_FLAG 
        + "_rep" + std::to_string(repetitions_over_array)
        + "_size-" + array_len_str;

  // Name displayed as plot title
  bench_name  = "find_if "
              + kwk::bench::bench_type_to_str(bench_type)
              + " " + mem_name
              + " " + pos_str;

  // If we are making a SYCL compute-bound benchmark, we need to find the right name for
  // the SYCL function being used (either std:: sycl:: or sycl::native::) 
  // if ((bench_type == kwk::bench::compute_CPU) ||(bench_type == kwk::bench::compute_GPU))
  // {
  //   #if KIWAKU_BENCH_KWK_SYCL_CPU || KIWAKU_BENCH_KWK_SYCL_GPU
  //     bench_name += " " + kwk::bench::trigo_function_to_fname(fct_type);
  //     fname += "_" + kwk::bench::trigo_function_to_fname(fct_type);
  //   #endif
  // }

  fname += ".bench";


  find_test<DATA_TYPE>( bench_name
                      , fname
                      , device_type
                      , bench_type
                      , convert_func
                      , convert_func_eve
                      , convert_func_sycl_base
                      , convert_func_sycl_native
                      , array_length
                      , repetitions_over_array
                      , put_at_pos
                      , clock_speed_CPU
                      );
  std::cout << "\n\n";
}

TTS_CASE("Benchmark - find_if Display")
{
  std::cout << "\n\n";
  std::cout << "KIWAKU_BENCH_COMPUTE_DATA_SIZE_MB = " << KIWAKU_BENCH_COMPUTE_DATA_SIZE_MB << "\n";
  std::cout << " KIWAKU_BENCH_MEMORY_DATA_SIZE_MB = " << KIWAKU_BENCH_MEMORY_DATA_SIZE_MB << "\n";
  std::cout << " KIWAKU_BENCH_CPU_CLOCK_SPEED_GHz = " << KIWAKU_BENCH_CPU_CLOCK_SPEED_GHz << "\n";
  std::cout << "          KIWAKU_BENCH_L2_SIZE_KB = " << KIWAKU_BENCH_L2_SIZE_KB << "\n";
  std::cout << "\n\n";

  TTS_EQUAL(true, true);
};

TTS_CASE("Benchmark - find_if memory-bound, last pos")
{
  INTERNAL_REPETITIONS = 1;
  // find_test_generic( kwk::bench::bench_type_t bench_type   // {compute_CPU, compute_GPU, memory, unknown}
  //   , kwk::bench::device_type_t device_type // {cpu, gpu, cpu_and_gpu}
  //   , kwk::bench::mem_type_t mem_type       // {RAM, L2, unknown_mem}
  //   , kwk::bench::trigo_function_t fct_type // {sycl_base, sycl_native, std_base}
  //   , auto convert_func
  //   , auto convert_func_eve
  //   , auto convert_func_sycl
  //   , find_test_pos pos
  //   )
  // Benchmarks for memory bandwidth of both CPU and GPU, if enabled in CMake

  auto convert_func = [](auto const& item) { return item; };

  find_test_generic ( kwk::bench::bench_type_t::memory
                    , kwk::bench::device_type_t::cpu_and_gpu
                    , kwk::bench::mem_type_t::RAM
                    // , kwk::bench::trigo_function_t::std_base // useless here
                    , convert_func
                    , convert_func // simd
                    , convert_func // sycl base
                    , convert_func // sycl native
                    , find_test_pos::last
                    );

  find_test_generic ( kwk::bench::bench_type_t::memory
                    , kwk::bench::device_type_t::cpu_and_gpu
                    , kwk::bench::mem_type_t::L2
                    // , kwk::bench::trigo_function_t::std_base // useless here
                    , convert_func
                    , convert_func // simd
                    , convert_func // sycl base
                    , convert_func // sycl native
                    , find_test_pos::last
                    );
};

TTS_CASE("Benchmark - find_if compute-bound CPU, last pos")
{
  INTERNAL_REPETITIONS = 1;

  auto convert_func = [](auto const& in)
  {
    return (std::cos(in * 0.67465f) * std::cos(in * 0.921546f) + std::sin(in * 0.543217f) * std::sin(in * 0.754878f) + 2);
  };

  auto convert_func_eve = [](auto const& in)
  {
    return (eve::cos(in * 0.67465f) * eve::cos(in * 0.921546f) + eve::sin(in * 0.543217f) * eve::sin(in * 0.754878f) + 2);
  };


  #if KIWAKU_BENCH_KWK_SYCL_CPU
    auto convert_func_sycl_base = [](auto const& in)
    {
      return  ::sycl::cos(in * 0.67465f) * ::sycl::cos(in * 0.921546f) + ::sycl::sin(in * 0.543217f) * ::sycl::sin(in * 0.754878f) + 2;
    };
    auto convert_func_sycl_native = [](auto const& in)
    {
      return  ::sycl::native::cos(in * 0.67465f) * ::sycl::native::cos(in * 0.921546f) + ::sycl::native::sin(in * 0.543217f) * ::sycl::native::sin(in * 0.754878f) + 2;
    };
    // With std::, large array stored in RAM
    find_test_generic ( kwk::bench::bench_type_t::compute_CPU
                      , kwk::bench::device_type_t::cpu
                      , kwk::bench::mem_type_t::RAM
                      // , kwk::bench::trigo_function_t::std_base
                      , convert_func
                      , convert_func_eve
                      , convert_func_sycl_base
                      , convert_func_sycl_native
                      , find_test_pos::last
                      );

    // With std::, smaller array that fits into L2 CPU cache
    find_test_generic ( kwk::bench::bench_type_t::compute_CPU
                      , kwk::bench::device_type_t::cpu
                      , kwk::bench::mem_type_t::L2
                      // , kwk::bench::trigo_function_t::std_base
                      , convert_func
                      , convert_func_eve
                      , convert_func_sycl_base
                      , convert_func_sycl_native
                      , find_test_pos::last
                      );
  #else
    // CPU and no SYCL
    // Large array stored in RAM
    find_test_generic ( kwk::bench::bench_type_t::compute_CPU
                      , kwk::bench::device_type_t::cpu
                      , kwk::bench::mem_type_t::RAM
                      // , kwk::bench::trigo_function_t::std_base
                      , convert_func
                      , convert_func_eve
                      , convert_func // no SYCL
                      , convert_func // no SYCL
                      , find_test_pos::last
                      );

    // Smaller array that fits into L2 CPU cache
    find_test_generic ( kwk::bench::bench_type_t::compute_CPU
                      , kwk::bench::device_type_t::cpu
                      , kwk::bench::mem_type_t::L2
                      // , kwk::bench::trigo_function_t::std_base
                      , convert_func
                      , convert_func_eve
                      , convert_func // no SYCL
                      , convert_func // no SYCL
                      , find_test_pos::last
                      );
  #endif

};

#if KIWAKU_BENCH_KWK_SYCL_GPU

  TTS_CASE("Benchmark - find_if compute-bound GPU, last pos")
  {
    const std::size_t repeat_trigo_op = 512;
    INTERNAL_REPETITIONS = repeat_trigo_op * 2; // sin + cos = 2 operations

    const float A = 2.71828f;
    const float B = 3.14159f;

    auto convert_func = [=](auto in)
    {
      float x = in;
      for (std::size_t i = 0; i < repeat_trigo_op; ++i)
        x = ::std::cos(x * A) + ::std::sin(x * B);
      return x;
    };

    auto convert_func_sycl_base = [=](auto in)
    {
      float x = in;
      for (std::size_t i = 0; i < repeat_trigo_op; ++i)
        x = ::sycl::cos(x * A) + ::sycl::sin(x * B);
      return x;
    };

    auto convert_func_sycl_native = [=](auto in)
    {
      float x = in;
      for (std::size_t i = 0; i < repeat_trigo_op; ++i)
        x = ::sycl::native::cos(x * A) + ::sycl::native::sin(x * B);
      return x;
    };

    // With std::, large array stored in RAM
    find_test_generic ( kwk::bench::bench_type_t::compute_GPU
                      , kwk::bench::device_type_t::gpu
                      , kwk::bench::mem_type_t::RAM
                      , convert_func
                      , [](auto x){ return x; } // no simd
                      , convert_func_sycl_base
                      , convert_func_sycl_native
                      , find_test_pos::last
                      );

    // With std::, smaller array that fits into L2 CPU cache
    find_test_generic ( kwk::bench::bench_type_t::compute_GPU
                      , kwk::bench::device_type_t::gpu
                      , kwk::bench::mem_type_t::L2
                      , convert_func
                      , [](auto x){ return x; } // no simd
                      , convert_func_sycl_base
                      , convert_func_sycl_native
                      , find_test_pos::last
                      );
  };

#endif // KIWAKU_BENCH_KWK_SYCL_GPU



#endif // KIWAKU_BENCH_BUILD