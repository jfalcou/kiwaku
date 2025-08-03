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
#include <kwk/algorithm/algos/transform.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <numeric>
#include <optional>
#include <math.h>
#include <cmath>
#include <execution> // don't forget the -ltbb compiler flag
#include <eve/module/math.hpp>

enum data_reset_t { trigo, ones };

template<typename DATA_TYPE>
void transform_test ( std::string const& bench_name
                    , std::string const& file_name
                    , auto func_generic
                    , [[maybe_unused]] auto func_eve
                    , std::size_t const L2_length // number of elements contained in L2 cache (number of values, NOT size in bytes)
                    , std::size_t const repetitions_over_array
                    , const data_reset_t data_reset
                    , ::kwk::bench::bench_type_t bench_type
                    , double clock_speed_CPU // expressed in GHz
                    , [[maybe_unused]] double clock_speed_GPU // expressed in GHz
                    , bool enable_gpu
                    )
{
  [[maybe_unused]] constexpr float MAX_ERROR = 1;
  constexpr bool enable_check = ENABLE_CHECK;

  // Total numer of element processed
  double total_number_of_elements_processed = L2_length * repetitions_over_array;
  double bandwidth_per_element_read   = sizeof(DATA_TYPE);
  double bandwidth_per_element_write  = sizeof(DATA_TYPE);
  double bandwidth_per_element_in_bytes_cpu = bandwidth_per_element_read + bandwidth_per_element_write;

  // std::size_t nb_iterations_gpu = 12;

  // double bandwidth_per_element_in_bytes_gpu = bandwidth_per_element_in_bytes_cpu * nb_iterations_gpu;

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


  // ====== Generic std function, used later ======
  auto fct_std_transform = [&](auto const& policy)
  {
    for (std::size_t repeat_co = 0; repeat_co < repetitions_over_array; ++repeat_co)
    {
      // [&](auto const re, auto const im) { return func_transform_re(re, im); }
      // Is function call expensive?
      std::transform( policy
                    , vector_inout.begin()
                    , vector_inout.end()
                    , vector_inout.begin()
                    , func_generic
                    );
    }
    return vector_inout[L2_length / 2];
  };


  // ====== Generic std benchmark ======
  [[maybe_unused]] auto bench_std = [&](auto const& policy, std::string policy_name)
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
  [[maybe_unused]] auto bench_kiwaku_cpu = [&](auto&& context, std::string context_name, auto fct)
  {
    DATA_TYPE return_;
    auto fct_generic = [&]()
    {
      // TODO: mettre compute_intensity plus haut
      for (std::size_t repeat_co = 0; repeat_co < repetitions_over_array; ++repeat_co)
      {
        // transform_inplace uniquement utile pour SYCL en vrai
        kwk::transform(context, fct, kwk_inout, kwk_inout);
      }
      return_ = kwk_inout(L2_length / 2);
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


  // #if ! KIWAKU_ONLY_BENCH_GPU

  // ====== Kiwaku SIMD ======
  // #if KIWAKU_BENCH_EVE
  //   b.set_iterations(1);
  //   bench_kiwaku_cpu(::kwk::simd, "kwk SIMD", func_eve);
  //   // if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_inout, truth_out, MAX_ERROR);
  // #endif

  // Remember output for later verification
  std::copy(vector_inout.begin(), vector_inout.end(), truth_out.begin());


  // ====== SYCL ======
  if (enable_gpu)
  {
    #if KIWAKU_BENCH_SYCL && TMP_ENABLE_GPU

      b.set_iterations(4);
    
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

        #define SKIP_DUMB_GPU false


        // Kiwaku GPU without smart proxies
        #if ! SKIP_DUMB_GPU
        {
          auto fct_transforms = [&]()
          {
            for (std::size_t repeat_co = 0; repeat_co < repetitions_over_array; ++repeat_co)
            {
              kwk::transform_inplace(ctx_gpu, func_generic, kwk_inout);
            }
            double sum = 0;
            for (std::size_t i = 0; i < L2_length; ++i)
            {
              sum += kwk_inout(i);
            }
            // std::cout << "\n\n  v1   sum = : " << sum << "\n\n";
            return sum; //kwk_inout(L2_length / 2);
          };

          // //[&]{ reset_data(vector_inout); }
          // Ne reset pas les données pour avoir quelque chose de cohérent avec le calcul suivant ("smart")
          b.run_ext2( "kwk SYCL GPU dumb " + ctx_gpu.get_device_name()
                    , fct_transforms
                    , []{} 
                    , total_number_of_elements_processed
                    , bandwidth_per_element_in_bytes_cpu
                    , bench_type
                    , clock_speed_GPU
                    , ::kwk::bench::device_type_t::gpu
                    );

          double sum = 0;
          for (std::size_t i = 0; i < L2_length; ++i)
          {
            sum += kwk_inout(i);
          }
          std::cout << "\n\n  v1   sum = : " << sum << "\n\n";
          

          std::cout << "\n\n  v1  kwk_inout(L2_length / 2): " << kwk_inout(L2_length / 2) << "\n\n";
        } // Kiwaku proxy out of scope = SYCL buffer destruction = data back to host
        #endif

        // Kiwaku GPU with memory staying on GPU memory
        {
          std::string context_name = "kwk SYCL GPU smart " + ctx_gpu.get_device_name();
          // Only pays the 1st data transfer

          // 1st data transfer non counted
          reset_data(vector_inout);
          auto proxy_inout = ctx_gpu.inout(kwk_inout);
          // kwk::transform_inplace_proxy(ctx_gpu, func_generic, proxy_inout);
          
          auto fct_transforms = [&]()
          {
            // sans le * nb_iterations parce qu'il le fait tout seul
            for (std::size_t repeat_co = 0; repeat_co < repetitions_over_array; ++repeat_co)
            {
              kwk::transform_inplace_proxy(ctx_gpu, func_generic, proxy_inout);
            }
            // std::cout << "Host code continues running..." << std::endl;
            // ctx_gpu.wait();
            // std::cout << "Host code continues running..." << std::endl;
            
            return 5; //kwk_inout(L2_length / 2);
          };


          b.run_ext2( context_name
                    , fct_transforms
                    , []{ } // pas de reset lorsqu'un chaîne les appels GPU sans remonter les données
                    // , [&]{ reset_data(vector_inout); }
                    , total_number_of_elements_processed
                    , bandwidth_per_element_in_bytes_cpu // Only to GPU local memory
                    , bench_type
                    , clock_speed_GPU
                    , ::kwk::bench::device_type_t::gpu
                    );

          ::sycl::host_accessor host_acc(proxy_inout.get_buffer(), sycl::read_only);
          double sum = 0;
          for (std::size_t i = 0; i < L2_length; ++i)
          {
            sum += host_acc[i];
          }
          std::cout << "\n\n  v2   sum = : " << sum << "\n\n";
          
          // std::cout << "\n\n  v2   sum = : " << sum << "\n\n";
          std::cout << "\n\n  v2   kwk_inout(L2_length / 2): " << kwk_inout(L2_length / 2) << "\n\n";
        } // Kiwaku proxy out of scope = SYCL buffer destruction = data back to host

      }
    #endif
  }



  [[maybe_unused]] auto concise = [] (double n) -> double
  {
    return std::round(n * 10) / 10;
  };

  // ====== hand-written sequential ======
  [[maybe_unused]] auto fct_hand = [&]()
  {
    for (std::size_t repeat_co = 0; repeat_co < repetitions_over_array; ++repeat_co)
    {
      for (std::size_t i = 0; i < L2_length; ++i)
      {
        vector_inout[i] = func_generic(vector_inout[i]);
      }
      for (std::size_t i = 0; i < 10; ++i) { std::cout << concise(vector_inout[i]) << " "; }
      std::cout << "\n";
    }
    return vector_inout[L2_length / 2];
  };

  reset_data(vector_inout);
  // fct_hand();



  #if ! KIWAKU_ONLY_BENCH_GPU

    
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
    bench_kiwaku_cpu(::kwk::cpu, "kwk CPU", func_generic);
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

    #endif //#if ! KIWAKU_ONLY_BENCH_GPU
  

  // TODO: reset data ????


  for (std::size_t i = 0; i < 30; ++i) { std::cout << truth_out[i] << " "; }
  std::cout << "\n";
  b.stop();
}


#define ENABLE_TRIGO false
#define ENABLE_MEMORY false

#define ENABLE_L2 false
#define ENABLE_RAM true

#define ENABLE_DESPAIR true



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
    // total_size = 32 * mio;
    total_size = 6 * gio;
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

  #if KIWAKU_ONLY_BENCH_GPU
    repetitions_over_array = 4;
  #endif


  std::cout << "\n======= REPEAT = " << repetitions_over_array << "\n\n";

  const float A = 2.71828f;
  const float B = 3.14159f;
  // const float A = 0.674651f;
  // const float B = 1.543217f;

  auto func = [A, B](auto in)
  {
    // return std::cos(in) * std::cos(in) + std::sin(in) * std::sin(in);
    // eve::cos(in) * eve::cos(in / 3) + eve::sin(in / 7) * eve::sin(in / 5);

    // return (std::cos(in * 0.67465f) * std::cos(in * 0.921546f) + std::sin(in * 0.543217f) * std::sin(in * 0.754878f)
    // + 2) ; // Entre (-1 et 1) * 2 = entre -2 et 2 + 2 -> entre 0 et 4 < 2 * PI.
    float x = in;
    // for (std::size_t i = 0; i < 128; ++i)
    //   x = ::sycl::cos(x * A) + ::sycl::sin(x * B);

    return x; //  (std::cos(in) + 2) * M_PI / 4 + 8 ; 
  };

// const float A = 2.71828f;
// const float B = 3.14159f;
// auto func = [A, B](auto in)
// {
//   float x = in;
//   for (std::size_t i = 0; i < 128; ++i)
//     x = ::sycl::cos(x * A) + ::sycl::sin(x * B);
//   return x;
// };

  auto func_eve = [](auto in)
  {
    // return eve::cos(in) * eve::cos(in) + eve::sin(in) * eve::sin(in);
    return (eve::cos(in * 0.67465f) * eve::cos(in * 0.921546f) + eve::sin(in * 0.543217f) * eve::sin(in * 0.754878f)
    + 2) ;
  };

  std::string l2_str = std::to_string(L2_length / kio);
  sutils::printer_t::head("Benchmark - transform, compute-bound (L2 " + l2_str + ")", true);

  std::string mem_name = "UNKNOWN MEMORY TYPE";
  if (mem_type == kwk::bench::mem_type_t::L2)  mem_name = "L2 cache";
  if (mem_type == kwk::bench::mem_type_t::RAM) mem_name = "RAM";

  transform_test<DATA_TYPE>( "transform compute-bound " + mem_name
                          , "transform_trigo_" + kwk::bench::EVE_COMPILER_FLAG + "_L2-" + l2_str + ".bench"
                          , func
                          , func_eve
                          , L2_length
                          , repetitions_over_array
                          , data_reset_t::trigo
                          , ::kwk::bench::bench_type_t::memory // compute todo
                          , clock_speed_CPU
                          , clock_speed_GPU
                          , true
                          );
  std::cout << "\n\n";
}



#if ENABLE_RAM
TTS_CASE("Benchmark - transform, compute-bound, RAM")
{
  compute_bound_test(kwk::bench::mem_type_t::RAM);
};
#endif // ENABLE_RAM


#if ENABLE_L2
TTS_CASE("Benchmark - transform, compute-bound, L2 cache")
{
  compute_bound_test(kwk::bench::mem_type_t::L2);
};
#endif // ENABLE_RAM

#endif // ENABLE_TRIGO



#if ENABLE_DESPAIR

void despair_bound_test(kwk::bench::mem_type_t mem_type)
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
    total_size = 4 * gio * kwk::bench::LEGEND_LOAD_FACTOR;
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

  // auto func     = [](auto in) { return in = in * 1.1f; };
  const float A = 2.71828f;
  const float B = 3.14159f;
  auto func = [A, B](auto in)
  {
    float x = in;
    for (std::size_t i = 0; i < 512; ++i)
      x = ::sycl::native::cos(x * A) + ::sycl::native::sin(x * B);

    return x; //  (std::cos(in) + 2) * M_PI / 4 + 8 ; 
  };
  auto func_eve = [](auto in) { return in = in * 1.1f; };

  std::string l2_str = std::to_string(L2_length / kio);
  sutils::printer_t::head("Benchmark - transform, memory-bound (L2 " + l2_str + ")", true);


  std::string mem_name = "UNKNOWN MEMORY TYPE";
  if (mem_type == kwk::bench::mem_type_t::L2)  mem_name = "L2 cache";
  if (mem_type == kwk::bench::mem_type_t::RAM) mem_name = "RAM";

  transform_test<DATA_TYPE>( "transform memory-bound " + mem_name
                          , "transform_memory_" + kwk::bench::EVE_COMPILER_FLAG + "_L2-" + l2_str + ".bench"
                          , func
                          , func_eve
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

TTS_CASE("Benchmark - transform, despair test")
{
  despair_bound_test(kwk::bench::mem_type_t::RAM);
};


#endif



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
    total_size = 4 * gio * kwk::bench::LEGEND_LOAD_FACTOR;
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

  auto func     = [](auto in) { return in = in * 1.1f; };
  auto func_eve = [](auto in) { return in = in * 1.1f; };

  std::string l2_str = std::to_string(L2_length / kio);
  sutils::printer_t::head("Benchmark - transform, memory-bound (L2 " + l2_str + ")", true);


  std::string mem_name = "UNKNOWN MEMORY TYPE";
  if (mem_type == kwk::bench::mem_type_t::L2)  mem_name = "L2 cache";
  if (mem_type == kwk::bench::mem_type_t::RAM) mem_name = "RAM";

  transform_test<DATA_TYPE>( "transform memory-bound " + mem_name
                          , "transform_memory_" + kwk::bench::EVE_COMPILER_FLAG + "_L2-" + l2_str + ".bench"
                          , func
                          , func_eve
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
TTS_CASE("Benchmark - transform, memory-bound RAM")
{
  memory_bound_test(kwk::bench::mem_type_t::RAM);
};
#endif // ENABLE_RAM


#if ENABLE_L2
TTS_CASE("Benchmark - transform, memory-bound L2 cache")
{
  memory_bound_test(kwk::bench::mem_type_t::L2);
};
#endif // ENABLE_RAM

#endif // ENABLE_MEMORY

#endif // KIWAKU_BUILD_BENCH