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

#include "include/benchmark.hpp"
#include "include/utils/utils.hpp"

// Temporaire pour aller plus vite
#define TMP_ENABLE_GPU true


#if KIWAKU_BENCH_SYCL
  #include <kwk/context/sycl/context.hpp>
#endif

#include <kwk/context/cpu/context.hpp>
#include <kwk/context/eve/context.hpp>
// #include <kwk/context/sycl/internal/sycl_tools.hpp>
#include <cstdlib>
#include <kwk/algorithm/algos/numeric.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <numeric>
#include <optional>
#include <cmath>
#include <execution> // don't forget the -ltbb compiler flag
#include <eve/module/math.hpp>

enum data_reset_t { trigo, ones };

#define REDUCE_NEUTRAL_ELEMENT 0

std::size_t INTERNAL_REPETITIONS = 1;
#define DESPAIR true

template<typename DATA_TYPE>
void transform_test ( std::string const& bench_name
                    , std::string const& file_name
                    , auto func_reduce
                    , auto func_transform_generic
                    , [[maybe_unused]] auto func_transform_eve
                    , std::size_t const L2_length // number of elements contained in L2 cache (number of values, NOT size in bytes)
                    , std::size_t const repetitions_over_array
                    , const data_reset_t data_reset
                    , ::kwk::bench::bench_type_t bench_type
                    , [[maybe_unused]] double clock_speed_CPU // expressed in GHz
                    , [[maybe_unused]] double clock_speed_GPU // expressed in GHz
                    , bool enable_gpu
                    )
{
  [[maybe_unused]] constexpr float MAX_ERROR = 1;
  constexpr bool enable_check = ENABLE_CHECK;

  // Total numer of element processed
  [[maybe_unused]] double total_number_of_elements_processed = L2_length * repetitions_over_array * INTERNAL_REPETITIONS;
  [[maybe_unused]] double bandwidth_per_element_read   = sizeof(DATA_TYPE);
  [[maybe_unused]] double bandwidth_per_element_write  = 0;
  [[maybe_unused]] double bandwidth_per_element_in_bytes_cpu = bandwidth_per_element_read + bandwidth_per_element_write;
  // On GPU it varies depending on what's being measured:
  // does buffers remain from one transform call to the next?

  // std::cout << "L2_length(" << L2_length << ") repeat_count(" << repeat_count << ")\n";

  // The algorithm writes into out and reads from in.
  std::vector<DATA_TYPE> vector_inout(L2_length);

  // Remember result for checking other values
  std::vector<DATA_TYPE> truth_out(L2_length);

  // Only used once now, since output and inputs are separated
  [[maybe_unused]] auto reset_data = [&](std::vector<DATA_TYPE>& victor)
  {
    if (data_reset == data_reset_t::ones)
    {
      std::fill(victor.begin(), victor.end(), 1);
    }
    if (data_reset == data_reset_t::trigo) 
    {
      std::mt19937 gen(78512); // rd()
      std::uniform_real_distribution<float> dis(0, 2.0 * M_PI);
      for (std::size_t i = 0; i < L2_length; ++i)
      {
        victor[i] = dis(gen);
      }
    }
  };

  if constexpr (!enable_check)
  {
    std::cout << "NO RESET DATA!\n";
    std::cout << "NO RESET DATA!\n";
    std::cout << "NO RESET DATA!\n";
    std::cout << "NO RESET DATA!\n";
    std::cout << "NO RESET DATA!\n";
    std::cout << "NO RESET DATA!\n";
    std::cout << "NO RESET DATA!\n";
    std::cout << "NO RESET DATA!\n";
  }

  // auto reset_data = [&] ([[maybe_unused]] std::vector<DATA_TYPE>& victor) {};

  reset_data(vector_inout);

  // Kiwaku views
  auto kwk_inout   = kwk::view{kwk::source = vector_inout.data()  , kwk::of_size(L2_length)};

  std::string y_axis_title = "UNKNOWN AXIS TYPE";

  if (bench_type == kwk::bench::bench_type_t::compute) y_axis_title = "Cycles per element";
  if (bench_type == kwk::bench::bench_type_t::memory)  y_axis_title = "Throughput"; // GB/s

  // Benchmark initialization
  kwk::bench::cbench_t b; // Processed elements, per second (higher is better)
  b.start ( kwk::bench::fprefix() + file_name
          , bench_name
          , y_axis_title
          , total_number_of_elements_processed
          , bench_type
          );
  // b.set_iterations(1);

  #if ! DESPAIR
  // ====== Generic std function, used later ======
  auto fct_std_transform = [&](auto const& policy)
  {
    DATA_TYPE res = REDUCE_NEUTRAL_ELEMENT;
    for (std::size_t repeat_co = 0; repeat_co < repetitions_over_array; ++repeat_co)
    {
      // [&](auto const re, auto const im) { return func_transform_re(re, im); }
      // Is function call expensive?
      res += std::transform_reduce( policy
                                  , vector_inout.begin()
                                  , vector_inout.end()
                                  , vector_inout.begin()
                                  , REDUCE_NEUTRAL_ELEMENT
                                  , func_reduce
                                  , func_transform_generic
                                  );
    }
    return res;
  };


  // ====== Generic std benchmark ======
  auto bench_std = [&](auto const& policy, std::string policy_name)
  {
    b.run_ext2( policy_name
              , [&]{ return fct_std_transform(policy); }
              , [&]{ reset_data(vector_inout); }
              , total_number_of_elements_processed
              , bandwidth_per_element_in_bytes_cpu
              , bench_type
              , clock_speed_CPU
              , ::kwk::bench::device_type_t::cpu
              );
  };


  // ====== Generic Kiwaku benchmark ======
  // -> DATA_TYPE
  // Should theoretically work for both CPU sequential and SIMD
  // (not for SYCL however)
  auto bench_kiwaku_cpu = [&](auto&& context, std::string context_name, auto transform_func_)
  {
    DATA_TYPE return_;
    auto fct_generic = [&]()
    {
      DATA_TYPE res = REDUCE_NEUTRAL_ELEMENT;
      // TODO: mettre compute_intensity plus haut
      for (std::size_t repeat_co = 0; repeat_co < repetitions_over_array; ++repeat_co)
      {
        // transform_inplace uniquement utile pour SYCL en vrai
        // func_reduce commune à EVE et les autres contextes Kiwaku
        res += kwk::transform_reduce_inplace(context, kwk_inout, REDUCE_NEUTRAL_ELEMENT, func_reduce, transform_func_);
        // res += kwk::transform_reduce(context, kwk_inout, kwk_inout, REDUCE_NEUTRAL_ELEMENT, func_reduce, transform_func_);
      }
      return_ = res;
      return return_;
    };
    b.run_ext2( context_name
              , fct_generic
              , [&]{ reset_data(vector_inout); }
              , total_number_of_elements_processed
              , bandwidth_per_element_in_bytes_cpu
              , bench_type
              , clock_speed_CPU
              , ::kwk::bench::device_type_t::cpu
              );
  };

  // ====== Generic Kiwaku benchmark ======
  // -> DATA_TYPE
  // Should theoretically work for both CPU sequential and SIMD
  // (not for SYCL however)
  auto bench_kiwaku_eve = [&](auto&& context, std::string context_name)
  {
    DATA_TYPE return_;
    auto fct_generic = [&]()
    {
      DATA_TYPE res = REDUCE_NEUTRAL_ELEMENT;
      // TODO: mettre compute_intensity plus haut
      for (std::size_t repeat_co = 0; repeat_co < repetitions_over_array; ++repeat_co)
      {
        // transform_inplace uniquement utile pour SYCL en vrai
        // func_reduce commune à EVE et les autres contextes Kiwaku
        res += kwk::transform_reduce(context, kwk_inout, kwk_inout, REDUCE_NEUTRAL_ELEMENT, std::pair{func_reduce, REDUCE_NEUTRAL_ELEMENT}, func_transform_eve);
      }
      return_ = res;
      return return_;
    };
    b.run_ext2( context_name
              , fct_generic
              , [&]{ reset_data(vector_inout); }
              , total_number_of_elements_processed
              , bandwidth_per_element_in_bytes_cpu
              , bench_type
              , clock_speed_CPU
              , ::kwk::bench::device_type_t::cpu
              );
  };


  // ====== Kiwaku SIMD ======
  #if KIWAKU_BENCH_EVE
    bench_kiwaku_eve(::kwk::simd, "kwk SIMD");
    // if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_inout, truth_out, MAX_ERROR);
  #endif

  // Remember output for later verification
  std::copy(vector_inout.begin(), vector_inout.end(), truth_out.begin());

  #endif // DESPAIR


  // ====== SYCL ======
  if (enable_gpu)
  {
    #if KIWAKU_BENCH_SYCL && TMP_ENABLE_GPU
    
      // OSEF pour le manuscrit
      // #if KIWAKU_BENCH_MTHREAD
      //   // ====== CPU ======
      //   auto& ctx = kwk::sycl::default_context;
      //   bench_kiwaku(ctx, "kwk SYCL " + ctx.get_device_name(), map_func);
      //   if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_inout, truth_out, MAX_ERROR);
      // #endif

      // Don't forget -fsycl-targets=nvptx64-nvidia-cuda,x86_64  (with x86_64 or spir64)
      bool has_gpu = kwk::sycl::has_gpu();

      // std::cout << "===== has gpu: " << ((has_gpu)?"yes":"no" )<< "\n";

      // Execute SYCL benchmark on GPU and CPU
      if (has_gpu)
      {
        // ====== Kiwaku SYCL GPU ======
        auto ctx_gpu = kwk::sycl::context{::sycl::gpu_selector_v};


        // Kiwaku GPU without smart proxies
        {
          auto fct_transforms = [&]()
          {
            DATA_TYPE res = REDUCE_NEUTRAL_ELEMENT;
            for (std::size_t repeat_co = 0; repeat_co < repetitions_over_array; ++repeat_co)
            {
              res += ctx_gpu.transform_reduce_inplace(kwk_inout, REDUCE_NEUTRAL_ELEMENT, func_reduce, func_transform_generic);
            }
            return res;
          };

          b.run_ext2( "kwk SYCL GPU dumb " + ctx_gpu.get_device_name()
                    , fct_transforms
                    , [&]{ reset_data(vector_inout); }
                    , total_number_of_elements_processed
                    , bandwidth_per_element_read + bandwidth_per_element_write
                    , bench_type
                    , clock_speed_GPU
                    , ::kwk::bench::device_type_t::gpu
                    );
        } // Kiwaku proxy out of scope = SYCL buffer destruction = data back to host


        // Kiwaku GPU with memory staying on GPU memory
        {
          std::string context_name = "kwk SYCL GPU smart " + ctx_gpu.get_device_name();
          // Only pays the 1st data transfer

          // 1st data transfer non counted
          auto proxy_inout = ctx_gpu.inout(kwk_inout);
          // kwk::transform_reduce_inplace_proxy(ctx_gpu, func_transform_generic, proxy_inout);
          // reset_data(vector_inout);
          
          auto fct_transforms = [&]()
          {
            DATA_TYPE res = REDUCE_NEUTRAL_ELEMENT;
            for (std::size_t repeat_co = 0; repeat_co < repetitions_over_array; ++repeat_co)
            {
              // kwk::transform_reduce_inplace_proxy(ctx_gpu, proxy_inout, REDUCE_NEUTRAL_ELEMENT, func_reduce, func_transform_generic);
              res += ctx_gpu.transform_reduce_inplace_proxy(kwk_inout, proxy_inout, REDUCE_NEUTRAL_ELEMENT, func_reduce, func_transform_generic);
            }
            return res;
          };

          b.run_ext2( context_name
                    , fct_transforms
                    , [&]{ reset_data(vector_inout); }
                    , total_number_of_elements_processed
                    , bandwidth_per_element_read + bandwidth_per_element_write // Only to GPU local memory
                    , bench_type
                    , clock_speed_GPU
                    , ::kwk::bench::device_type_t::gpu
                    );
        } // Kiwaku proxy out of scope = SYCL buffer destruction = data back to host

      }
    #endif
  }



  #if ! DESPAIR

  // ====== hand-written sequential ======
  auto fct_hand = [&]()
  {
    DATA_TYPE res = REDUCE_NEUTRAL_ELEMENT;
    for (std::size_t repeat_co = 0; repeat_co < repetitions_over_array; ++repeat_co)
    {
      for (std::size_t i = 0; i < L2_length; ++i)
      {
        res += func_reduce(res, func_transform_generic(vector_inout[i], vector_inout[i]));
      }
    }
    return res;
  };
  b.run_ext2( "hand CPU"
            , fct_hand
            , [&]{ reset_data(vector_inout); }
            , total_number_of_elements_processed
            , bandwidth_per_element_in_bytes_cpu
            , bench_type
            , clock_speed_CPU
            , ::kwk::bench::device_type_t::cpu
            );
  if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_inout, truth_out, MAX_ERROR);


  // ====== Kiwaku CPU context ======
  bench_kiwaku_cpu(::kwk::cpu, "kwk CPU", func_transform_generic);
  if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_inout, truth_out, MAX_ERROR);





  // ====== std sequential ======
  bench_std(std::execution::seq, "seq");
  if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_inout, truth_out, MAX_ERROR);


  // ====== std unsequenced ======
  bench_std(std::execution::unseq, "unseq");
  if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_inout, truth_out, MAX_ERROR);


  // Don't forget the -ltbb compiler flag
  #if ENABLE_TBB && KIWAKU_BENCH_MTHREAD

    // ====== std parallel ======
    bench_std(std::execution::par, "par");
    if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_inout, truth_out, MAX_ERROR);

    // ====== std parallel unsequenced ======
    bench_std(std::execution::par_unseq, "par_unseq");
    if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_inout, truth_out, MAX_ERROR);
  #endif

  #endif // #if ! DESPAIR

  

  // TODO: reset data ????


  for (std::size_t i = 0; i < 30; ++i) { std::cout << truth_out[i] << " "; }
  std::cout << "\n";
  b.stop();
}



#define ENABLE_TRIGO false
#define ENABLE_MEMORY true

#define ENABLE_L2 true
#define ENABLE_RAM true






#if DESPAIR

void compute_DESPAIR_test(kwk::bench::mem_type_t mem_type, kwk::bench::trigo_function_t fct_type)
{
  ::kwk::bench::get_eve_compiler_flag();
  using DATA_TYPE = float;
  [[maybe_unused]] std::size_t kio = 1024 / (sizeof(DATA_TYPE) * 1); // input + output
  [[maybe_unused]] std::size_t mio = 1024 * kio;
  [[maybe_unused]] std::size_t gio = 1024 * mio;
  std::size_t total_size = 0;
  std::size_t L2_length = 0; // L2 cache size, in bytes
  double clock_speed_CPU = 0;
  double clock_speed_GPU = 0;
  std::string hname = sutils::get_host_name();
  // Total data to process
  if (hname == "parsys-legend")
  {
    // total_size = 128 * mio;
    total_size = 1 * gio;
    L2_length = 256 * kio;
    // total_size = 256 * mio * kwk::bench::LEGEND_LOAD_FACTOR;
    // L2_length = total_size;
    clock_speed_CPU = 4.7;
    clock_speed_GPU = 1.6; // From 1.3 to 1.8
  }

  if (mem_type == kwk::bench::mem_type_t::RAM)
  {
    L2_length = total_size;
  }
  // et si kwk::bench::mem_type_t::L2, alors on garde le L2_length réel


  std::size_t repetitions_over_array = total_size / L2_length; // Number of repetitions
  std::cout << "\n======= REPEAT = " << repetitions_over_array << "\n\n";

  // auto func_transform = [](auto in1, auto in2)
  // {
  //   // return std::cos(in) * std::cos(in) + std::sin(in) * std::sin(in);
  //   // eve::cos(in) * eve::cos(in / 3) + eve::sin(in / 7) * eve::sin(in / 5);
  //   return (std::cos(in1 * 0.67465f) * std::cos(in1 * 0.921546f) + std::sin(in2 * 0.543217f) * std::sin(in2 * 0.754878f)
  //           + 2) ; // Entre (-1 et 1) * 2 = entre -2 et 2 + 2 -> entre 0 et 4 < 2 * PI.
  // };

  auto func_transform_eve = [](auto in1, auto in2)
  {
    // return eve::cos(in) * eve::cos(in) + eve::sin(in) * eve::sin(in);
    return (eve::cos(in1 * 0.67465f) * eve::cos(in1 * 0.921546f) + eve::sin(in2 * 0.543217f) * eve::sin(in2 * 0.754878f)
    + 2) ;
  };

  auto func_reduce = [](auto in1, auto in2) { return in1 + in2; };

  std::string l2_str = std::to_string(L2_length / kio);
  sutils::printer_t::head("Benchmark - transform, compute-bound (L2 " + l2_str + ")", true);

  std::string mem_name = "UNKNOWN MEMORY TYPE";
  if (mem_type == kwk::bench::mem_type_t::L2)  mem_name = "L2 cache";
  if (mem_type == kwk::bench::mem_type_t::RAM) mem_name = "RAM";

  const std::size_t repeat_trigo_op = 512;
  INTERNAL_REPETITIONS = repeat_trigo_op * 2; // sin + cos = 2 operations

  const float A = 2.71828f;
  const float B = 3.14159f;
  // const float A = 0.674651f;
  // const float B = 1.543217f;

  std::string sycl_fname =  kwk::bench::trigo_function_to_fname(fct_type);

  if (fct_type == kwk::bench::trigo_function_t::sycl_base)
  {
    auto func_transform = [A, B](auto in1, auto in2)
    {
      float x = in1 + in2;
      for (std::size_t i = 0; i < repeat_trigo_op; ++i)
        x = ::sycl::cos(x * A) + ::sycl::sin(x * B);

      return x; //  (std::cos(in) + 2) * M_PI / 4 + 8 ; 
    };
    transform_test<DATA_TYPE>( "transform_reduce "+ sycl_fname + "-bound " + mem_name
                            , "transform_reduce_"+ sycl_fname + "_" + kwk::bench::EVE_COMPILER_FLAG + "_L2-" + l2_str + ".bench"
                            , func_reduce
                            , func_transform
                            , func_transform_eve
                            , L2_length
                            , repetitions_over_array
                            , data_reset_t::trigo
                            , ::kwk::bench::bench_type_t::GPU_compute
                            , clock_speed_CPU
                            , clock_speed_GPU
                            , true
                            );
  }

  if (fct_type == kwk::bench::trigo_function_t::sycl_native)
  {
    auto func_transform = [A, B](auto in1, auto in2)
    {
      float x = in1 + in2;
      for (std::size_t i = 0; i < repeat_trigo_op; ++i)
        x = ::sycl::native::cos(x * A) + ::sycl::native::sin(x * B);

      return x; //  (std::cos(in) + 2) * M_PI / 4 + 8 ; 
    };
    transform_test<DATA_TYPE>( "transform_reduce "+ sycl_fname + "-bound " + mem_name
                            , "transform_reduce_"+ sycl_fname + "_" + kwk::bench::EVE_COMPILER_FLAG + "_L2-" + l2_str + ".bench"
                            , func_reduce
                            , func_transform
                            , func_transform_eve
                            , L2_length
                            , repetitions_over_array
                            , data_reset_t::trigo
                            , ::kwk::bench::bench_type_t::GPU_compute
                            , clock_speed_CPU
                            , clock_speed_GPU
                            , true
                            );
  }

  if (fct_type == kwk::bench::trigo_function_t::std_base)
  {
    auto func_transform = [A, B](auto in1, auto in2)
    {
      float x = in1 + in2;
      for (std::size_t i = 0; i < repeat_trigo_op; ++i)
        x = ::std::cos(x * A) + ::std::sin(x * B);

      return x; //  (std::cos(in) + 2) * M_PI / 4 + 8 ; 
    };
    transform_test<DATA_TYPE>( "transform_reduce "+ sycl_fname + "-bound " + mem_name
                            , "transform_reduce_"+ sycl_fname + "_" + kwk::bench::EVE_COMPILER_FLAG + "_L2-" + l2_str + ".bench"
                            , func_reduce
                            , func_transform
                            , func_transform_eve
                            , L2_length
                            , repetitions_over_array
                            , data_reset_t::trigo
                            , ::kwk::bench::bench_type_t::GPU_compute
                            , clock_speed_CPU
                            , clock_speed_GPU
                            , true
                            );
  }

  
  std::cout << "\n\n";
}

TTS_CASE("Benchmark - transform_reduce, DESPAIR-bound, RAM")
{
  compute_DESPAIR_test(kwk::bench::mem_type_t::RAM, kwk::bench::trigo_function_t::sycl_native);
  compute_DESPAIR_test(kwk::bench::mem_type_t::RAM, kwk::bench::trigo_function_t::sycl_base);
  compute_DESPAIR_test(kwk::bench::mem_type_t::RAM, kwk::bench::trigo_function_t::std_base);
};

#endif // #if DESPAIR






#if ! DESPAIR


#if ENABLE_TRIGO


void compute_bound_test(kwk::bench::mem_type_t mem_type)
{
  ::kwk::bench::get_eve_compiler_flag();
  using DATA_TYPE = float;
  [[maybe_unused]] std::size_t kio = 1024 / (sizeof(DATA_TYPE) * 1); // input + output
  [[maybe_unused]] std::size_t mio = 1024 * kio;
  [[maybe_unused]] std::size_t gio = 1024 * mio;
  std::size_t total_size = 0;
  std::size_t L2_length = 0; // L2 cache size, in bytes
  double clock_speed_CPU = 0;
  double clock_speed_GPU = 0;
  std::string hname = sutils::get_host_name();
  // Total data to process
  if (hname == "parsys-legend")
  {
    // total_size = 128 * mio;
    total_size = 1 * gio;
    L2_length = 256 * kio;
    // total_size = 256 * mio * kwk::bench::LEGEND_LOAD_FACTOR;
    // L2_length = total_size;
    clock_speed_CPU = 4.7;
    clock_speed_GPU = 1.6; // From 1.3 to 1.8
  }

  if (mem_type == kwk::bench::mem_type_t::RAM)
  {
    L2_length = total_size;
  }
  // et si kwk::bench::mem_type_t::L2, alors on garde le L2_length réel


  std::size_t repetitions_over_array = total_size / L2_length; // Number of repetitions
  std::cout << "\n======= REPEAT = " << repetitions_over_array << "\n\n";

  auto func_transform = [](auto in1, auto in2)
  {
    // return std::cos(in) * std::cos(in) + std::sin(in) * std::sin(in);
    // eve::cos(in) * eve::cos(in / 3) + eve::sin(in / 7) * eve::sin(in / 5);
    return (std::cos(in1 * 0.67465f) * std::cos(in1 * 0.921546f) + std::sin(in2 * 0.543217f) * std::sin(in2 * 0.754878f)
            + 2) ; // Entre (-1 et 1) * 2 = entre -2 et 2 + 2 -> entre 0 et 4 < 2 * PI.
  };
  auto func_transform_eve = [](auto in1, auto in2)
  {
    // return eve::cos(in) * eve::cos(in) + eve::sin(in) * eve::sin(in);
    return (eve::cos(in1 * 0.67465f) * eve::cos(in1 * 0.921546f) + eve::sin(in2 * 0.543217f) * eve::sin(in2 * 0.754878f)
    + 2) ;
  };

  auto func_reduce = [](auto in1, auto in2) { return in1 + in2; };

  std::string l2_str = std::to_string(L2_length / kio);
  sutils::printer_t::head("Benchmark - transform, compute-bound (L2 " + l2_str + ")", true);

  std::string mem_name = "UNKNOWN MEMORY TYPE";
  if (mem_type == kwk::bench::mem_type_t::L2)  mem_name = "L2 cache";
  if (mem_type == kwk::bench::mem_type_t::RAM) mem_name = "RAM";


  transform_test<DATA_TYPE>( "transform_reduce compute-bound " + mem_name
                          , "transform_reduce_trigo_" + kwk::bench::EVE_COMPILER_FLAG + "_L2-" + l2_str + ".bench"
                          , func_reduce
                          , func_transform
                          , func_transform_eve
                          , L2_length
                          , repetitions_over_array
                          , data_reset_t::trigo
                          , ::kwk::bench::bench_type_t::compute
                          , clock_speed_CPU
                          , clock_speed_GPU
                          , true
                          );
  std::cout << "\n\n";
}



#if ENABLE_RAM
TTS_CASE("Benchmark - transform_reduce, compute-bound, RAM")
{
  compute_bound_test(kwk::bench::mem_type_t::RAM);
};
#endif // ENABLE_RAM


#if ENABLE_L2
TTS_CASE("Benchmark - transform_reduce, compute-bound, L2 cache")
{
  compute_bound_test(kwk::bench::mem_type_t::L2);
};
#endif // ENABLE_RAM

#endif // ENABLE_TRIGO





#if ENABLE_MEMORY

void memory_bound_test(kwk::bench::mem_type_t mem_type)
{
  ::kwk::bench::get_eve_compiler_flag();
  using DATA_TYPE = float;
  [[maybe_unused]] std::size_t kio = 1024 / (sizeof(DATA_TYPE) * 1); // single input
  [[maybe_unused]] std::size_t mio = 1024 * kio;
  [[maybe_unused]] std::size_t gio = 1024 * mio;
  std::size_t total_size = 0;
  std::size_t L2_length = 0; // L2 cache size, in bytes
  double clock_speed_CPU = 0;
  double clock_speed_GPU = 0;
  std::string hname = sutils::get_host_name();

  if (hname == "parsys-legend")
  {
    // total_size = 1 * gio;
    total_size = 6 * gio;
    L2_length = 256 * kio;
    clock_speed_CPU = 4.7;
    clock_speed_GPU = 1.6; // From 1.3 to 1.8
  } 

  if (mem_type == kwk::bench::mem_type_t::RAM)
  {
    L2_length = total_size;
  }


  std::size_t repetitions_over_array = total_size / L2_length; // Number of repetitions
  std::cout << "\n======= REPEAT = " << repetitions_over_array << "\n\n";

  auto func_transform     = [](auto in1, auto in2) { return in1 + in2; };
  auto func_transform_eve = [](auto in1, auto in2) { return in1 + in2; };

  auto func_reduce = [](auto in1, auto in2) { return in1 + in2; };

  std::string l2_str = std::to_string(L2_length / kio);
  sutils::printer_t::head("Benchmark - transform_reduce, memory-bound (L2 " + l2_str + ")", true);


  std::string mem_name = "UNKNOWN MEMORY TYPE";
  if (mem_type == kwk::bench::mem_type_t::L2)  mem_name = "L2 cache";
  if (mem_type == kwk::bench::mem_type_t::RAM) mem_name = "RAM";

  transform_test<DATA_TYPE>( "transform_reduce memory-bound " + mem_name
                          , "transform_reduce_memory_" + kwk::bench::EVE_COMPILER_FLAG + "_L2-" + l2_str + ".bench"
                          , func_reduce
                          , func_transform
                          , func_transform_eve
                          , L2_length
                          , repetitions_over_array
                          , data_reset_t::ones
                          , ::kwk::bench::bench_type_t::memory
                          , clock_speed_CPU
                          , clock_speed_GPU
                          , true
                          );
  std::cout << "\n\n";
}



#if ENABLE_RAM
TTS_CASE("Benchmark - transform_reduce, memory-bound RAM")
{
  memory_bound_test(kwk::bench::mem_type_t::RAM);
};
#endif // ENABLE_RAM


#if ENABLE_L2
TTS_CASE("Benchmark - transform_reduce, memory-bound L2 cache")
{
  memory_bound_test(kwk::bench::mem_type_t::L2);
};
#endif // ENABLE_RAM

#endif // ENABLE_MEMORY

#endif // #if ! DESPAIR

#endif // KIWAKU_BUILD_BENCH