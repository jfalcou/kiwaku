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
#include <cmath>
#include <execution> // don't forget the -ltbb compiler flag
#include <eve/module/math.hpp>

enum data_reset_t { trigo, ones };

template<typename DATA_TYPE>
void transform_test ( std::string const& bench_name
                    , std::string const& file_name
                    , auto func_generic
                    , auto func_eve
                    , std::size_t const L2_length // number of elements contained in L2 cache (number of values, NOT size in bytes)
                    , std::size_t const repetitions_over_array
                    , const data_reset_t data_reset
                    , ::kwk::bench::bench_type_t bench_type
                    , double clock_speed_CPU // expressed in GHz
                    , [[maybe_unused]] double clock_speed_GPU // expressed in GHz
                    , bool enable_gpu
                    )
{
  constexpr float MAX_ERROR = 1;
  constexpr bool enable_check = ENABLE_CHECK;

  // Total numer of element processed
  double total_number_of_elements_processed = L2_length * repetitions_over_array;
  double bandwidth_per_element_read   = sizeof(DATA_TYPE);
  double bandwidth_per_element_write  = sizeof(DATA_TYPE);
  double bandwidth_per_element_in_bytes_cpu = bandwidth_per_element_read + bandwidth_per_element_write;
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
  auto bench_kiwaku_cpu = [&](auto&& context, std::string context_name, auto fct)
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



  // ====== Kiwaku SIMD ======
  #if KIWAKU_BENCH_EVE
    bench_kiwaku_cpu(::kwk::simd, "kwk SIMD", func_eve);
    // if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_inout, truth_out, MAX_ERROR);
  #endif

  // Remember output for later verification
  std::copy(vector_inout.begin(), vector_inout.end(), truth_out.begin());


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
            for (std::size_t repeat_co = 0; repeat_co < repetitions_over_array; ++repeat_co)
            {
              kwk::transform_inplace(ctx_gpu, func_generic, kwk_inout);
            }
            return kwk_inout(L2_length / 2);
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
          kwk::transform_inplace_proxy(ctx_gpu, func_generic, proxy_inout);
          reset_data(vector_inout);
          
          auto fct_transforms = [&]()
          {
            for (std::size_t repeat_co = 0; repeat_co < repetitions_over_array; ++repeat_co)
            {
              kwk::transform_inplace_proxy(ctx_gpu, func_generic, proxy_inout);
            }
            return kwk_inout(L2_length / 2);
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





  // ====== hand-written sequential ======
  auto fct_hand = [&]()
  {
    for (std::size_t repeat_co = 0; repeat_co < repetitions_over_array; ++repeat_co)
    {
      for (std::size_t i = 0; i < L2_length; ++i)
      {
        vector_inout[i] = func_generic(vector_inout[i]);
      }
    }
    return vector_inout[L2_length / 2];
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



  

  // TODO: reset data ????


  for (std::size_t i = 0; i < 30; ++i) { std::cout << truth_out[i] << " "; }
  std::cout << "\n";
  b.stop();
}


#define ENABLE_TRIGO true
#define ENABLE_MEMORY true

#define ENABLE_L2 true
#define ENABLE_RAM true



#if ENABLE_TRIGO
// TTS_CASE("Benchmark - transform, compute-bound")
// {
//   ::kwk::bench::get_eve_compiler_flag();

//   using DATA_TYPE = float;

//   // const DATA_TYPE PI = static_cast<DATA_TYPE>(M_PI);



//   [[maybe_unused]] std::size_t kio = 1024 / (sizeof(DATA_TYPE) * 1); // input + output
//   [[maybe_unused]] std::size_t mio = 1024 * kio;
//   [[maybe_unused]] std::size_t gio = 1024 * mio;

//   std::size_t total_size;
//   std::size_t L2_length; // L2 cache size, in bytes
//   std::string hname = sutils::get_host_name();
//   // Total data to process
//         if (hname == "parsys-legend")          { total_size = 256 * mio * kwk::bench::LEGEND_LOAD_FACTOR; L2_length = total_size; } 
//         // if (hname == "parsys-legend")          { L2_length = 512 * kio; total_size = 1 * gio * kwk::bench::LEGEND_LOAD_FACTOR; } 

//   else if (hname == "pata")                   { total_size = 128 * mio; L2_length = total_size; }
//   else if (hname == "chaton")                 { total_size = 32 * mio; L2_length = total_size; }
//   // else if (hname == "chaton")                 { L2_length = 256 * kio; total_size = 128 * mio; }
//   else if (hname == "sylvain-ThinkPad-T580")  { L2_length = 256 * kio; total_size = 8 * mio; }

//   // 512 kio par L2
//   else if (hname == "falcou-avx512")          { total_size = 512 * mio;  L2_length = total_size; //128 * kio; 
//                                                 std::cout << "OK, Used computer: falcou-avx512\n"; }

//   // La Pierre
//   else if (hname == "lapierre")               { L2_length = 128 * mio; total_size = 256 * mio; }
//   else                                        { L2_length = 256 * kio; total_size = 8 * mio; }

//   std::size_t repetitions = total_size / L2_length; // Number of repetitions

//   std::cout << "\n======= REPEAT = " << repetitions << "\n\n";


//   // === Unitary complex ===


//   // const std::size_t repeat_inner = 10;
//   // auto map_func = [=](DATA_TYPE& re, DATA_TYPE& im)
//   // {
//     // for (std::size_t i = 0; i < repeat_inner; ++i) {
//       // DATA_TYPE re_ = re;
//       // DATA_TYPE im_ = im;
//       // re = re_ * re_ - im_ * im_;
//       // im = re_ * im_ + im_ * re_;
//     // }
//   // };
//   // auto map_func_eve = map_func;




//   // // TODO: Allouer les tableaux in_re, inout_im, out_re
//   // for (std::size_t i = 0; i < repeat_inner; ++i) {
//   //   kwk::map(out_re, [](auto re, auto im) { return re * re - im * im; },
//   //            in_re, inout_im);
//   //   kwk::map(inout_im,
//   //            [](auto re, auto im) { return re * im + im * re; },
//   //            in_re, inout_im);
//   //   std::swap(in_re, out_re);
//   // }

//   // kwk::transform(::kwk::simd, map_func_, kwk_re, kwk_im);
//   // auto map_func_eve = [=](auto& re, auto& im)
//   // {
    
//   //   re = ::eve::cos(re) * ::eve::cos(re) + ::eve::sin(re) * ::eve::sin(re);
//   //   im = ::eve::cos(im) * ::eve::cos(im) + ::eve::sin(im) * ::eve::sin(im);
//   // };


//   // kwk::transform(::kwk::simd, map_func_, kwk_re, kwk_im);
//   // auto map_func_eve = [=](auto& re, auto& im)
//   // {
//   //   for (std::size_t r = 0; r < repetitions; ++r)
//   //   {
//   //     re = ::eve::cos(re) * ::eve::cos(re) + ::eve::sin(re) * ::eve::sin(re);
//   //     im = ::eve::cos(im) * ::eve::cos(im) + ::eve::sin(im) * ::eve::sin(im);
//   //   }
//   // };

//   // const std::size_t compute_intensity = 1;

//   auto func = [](auto in)
//   {
//     return std::cos(in) * std::cos(in) + std::sin(in) * std::sin(in);
//   };

//   auto func_eve = [](auto in)
//   {
//     return eve::cos(in) * eve::cos(in) + eve::sin(in) * eve::sin(in);
//   };



//   std::string l2_str = std::to_string(L2_length / kio);

  


//   sutils::printer_t::head("Benchmark - transform, compute-bound (L2 " + l2_str + ")", true);

//   transform_test<DATA_TYPE>( "transform trigo_inpl"
//                           , "transform_trigo_" + kwk::bench::EVE_COMPILER_FLAG + "_L2-" + l2_str + ".bench"
//                           , func
//                           , func_eve
//                           , L2_length
//                           , L2_length * repetitions
//                           , data_reset_t::trigo
//                           , repetitions
//                           );
//   std::cout << "\n\n";
// };

#if ENABLE_RAM
TTS_CASE("Benchmark - transform, compute-bound, RAM")
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
    total_size = 4 * gio;
    // total_size = 256 * mio * kwk::bench::LEGEND_LOAD_FACTOR;
    L2_length = total_size;
    clock_speed_CPU = 4.7;
    clock_speed_GPU = 1.6; // From 1.3 to 1.8
  } 

  std::size_t repetitions_over_array = total_size / L2_length; // Number of repetitions
  std::cout << "\n======= REPEAT = " << repetitions_over_array << "\n\n";

  auto func = [](auto in)
  {
    // return std::cos(in) * std::cos(in) + std::sin(in) * std::sin(in);
    // eve::cos(in) * eve::cos(in / 3) + eve::sin(in / 7) * eve::sin(in / 5);
    return (std::cos(in * 0.67465f) * std::cos(in * 0.921546f) + std::sin(in * 0.543217f) * std::sin(in * 0.754878f)
            + 2) ; // Entre (-1 et 1) * 2 = entre -2 et 2 + 2 -> entre 0 et 4 < 2 * PI.
  };
  auto func_eve = [](auto in)
  {
    // return eve::cos(in) * eve::cos(in) + eve::sin(in) * eve::sin(in);
    return (eve::cos(in * 0.67465f) * eve::cos(in * 0.921546f) + eve::sin(in * 0.543217f) * eve::sin(in * 0.754878f)
    + 2) ;
  };

  std::string l2_str = std::to_string(L2_length / kio);
  sutils::printer_t::head("Benchmark - transform, compute-bound (L2 " + l2_str + ")", true);

  transform_test<DATA_TYPE>( "transform compute-bound RAM"
                          , "transform_trigo_RAM_" + kwk::bench::EVE_COMPILER_FLAG + "_L2-" + l2_str + ".bench"
                          , func
                          , func_eve
                          , L2_length
                          , repetitions_over_array
                          , data_reset_t::trigo
                          , ::kwk::bench::bench_type_t::compute
                          , clock_speed_CPU
                          , clock_speed_GPU
                          , true
                          );
  std::cout << "\n\n";
};
#endif // ENABLE_RAM

#if ENABLE_L2
TTS_CASE("Benchmark - transform, compute-bound, L2")
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
    total_size = 4 * gio;
    // total_size = 256 * mio * kwk::bench::LEGEND_LOAD_FACTOR;
    L2_length = 256 * kio;
    clock_speed_CPU = 4.7;
    clock_speed_GPU = 1.6; // From 1.3 to 1.8
  } 

  std::size_t repetitions_over_array = total_size / L2_length; // Number of repetitions
  std::cout << "\n======= REPEAT = " << repetitions_over_array << "\n\n";

  auto func = [](auto in)
  {
    // return std::cos(in) * std::cos(in) + std::sin(in) * std::sin(in);
    // eve::cos(in) * eve::cos(in / 3) + eve::sin(in / 7) * eve::sin(in / 5);
    return (std::cos(in * 0.67465f) * std::cos(in * 0.921546f) + std::sin(in * 0.543217f) * std::sin(in * 0.754878f)
            + 2) ; // Entre (-1 et 1) * 2 = entre -2 et 2 + 2 -> entre 0 et 4 < 2 * PI.
  };
  auto func_eve = [](auto in)
  {
    // return eve::cos(in) * eve::cos(in) + eve::sin(in) * eve::sin(in);
    return (eve::cos(in * 0.67465f) * eve::cos(in * 0.921546f) + eve::sin(in * 0.543217f) * eve::sin(in * 0.754878f)
    + 2) ;
  };

  std::string l2_str = std::to_string(L2_length / kio);
  sutils::printer_t::head("Benchmark - transform, compute-bound (L2 " + l2_str + ")", true);

  transform_test<DATA_TYPE>( "transform compute-bound L2 cache"
                          , "transform_trigo_L2_" + kwk::bench::EVE_COMPILER_FLAG + "_L2-" + l2_str + ".bench"
                          , func
                          , func_eve
                          , L2_length
                          , repetitions_over_array
                          , data_reset_t::trigo
                          , ::kwk::bench::bench_type_t::compute
                          , clock_speed_CPU
                          , clock_speed_GPU
                          , true
                          );
  std::cout << "\n\n";
};
#endif // ENABLE_L2
#endif // ENABLE_TRIGO





#if ENABLE_MEMORY
// TTS_CASE("Benchmark - transform, memory-bound")
// {
//   ::kwk::bench::get_eve_compiler_flag();

//   using DATA_TYPE = float;

//   [[maybe_unused]] std::size_t kio = 1024 / (sizeof(DATA_TYPE) * 1); // single input
//   [[maybe_unused]] std::size_t mio = 1024 * kio;
//   [[maybe_unused]] std::size_t gio = 1024 * mio;

//   std::size_t total_size;
//   std::size_t L2_length; // L2 cache size, in bytes
//   std::string hname = sutils::get_host_name();
//   // Total data to process
//   // if (hname == "parsys-legend")          { total_size = 2 ; L2_length = total_size; } 
  
//   if (hname == "parsys-legend")
//   {
//     total_size = 4 * gio * kwk::bench::LEGEND_LOAD_FACTOR;
//     L2_length = total_size;
//   } 
//         // if (hname == "parsys-legend")          { L2_length = 512 * kio; total_size = 1 * gio * kwk::bench::LEGEND_LOAD_FACTOR; } 

//   else if (hname == "pata")                   { total_size = 128 * mio; L2_length = total_size; }
//   else if (hname == "chaton")                 { total_size = 64 * mio; L2_length = total_size; } // 
//   // else if (hname == "chaton")                 { L2_length = 256 * kio; total_size = 128 * mio; }
//   else if (hname == "sylvain-ThinkPad-T580")  { L2_length = 256 * kio; total_size = 8 * mio; }

//   // 512 kio par L2
//   else if (hname == "falcou-avx512")          { total_size = 512 * mio;  L2_length = total_size; //128 * kio; 
//                                                 std::cout << "OK, Used computer: falcou-avx512\n"; }

//   // La Pierre
//   else if (hname == "lapierre")               { L2_length = 128 * mio; total_size = 256 * mio; }
//   else                                        { L2_length = 256 * kio; total_size = 8 * mio; }

//   std::size_t repetitions = total_size / L2_length; // Number of repetitions

//   std::cout << "\n======= REPEAT = " << repetitions << "\n\n";


//   // const std::size_t compute_intensity_local = 1;

//   auto func = [](auto in)
//   {
//     return in = in * 1.1;
//   };

//   auto func_eve = [](auto in)
//   {
//     return in = in * 1.1;
//   };



//   std::string l2_str = std::to_string(L2_length / kio);

//   const std::size_t compute_intensity = 1;


//   sutils::printer_t::head("Benchmark - transform, memory-bound (L2 " + l2_str + ")", true);

//   transform_test<DATA_TYPE>( "transform memory-bound"
//                           , "transform_memory_" + kwk::bench::EVE_COMPILER_FLAG + "_L2-" + l2_str + ".bench"
//                           , func
//                           , func_eve
//                           , L2_length
//                           , L2_length * repetitions
//                           , data_reset_t::ones
//                           , repetitions
//                           );
//   std::cout << "\n\n";
// };

#if ENABLE_RAM
TTS_CASE("Benchmark - transform, memory-bound RAM")
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
    L2_length = total_size;
    clock_speed_CPU = 4.7;
    clock_speed_GPU = 1.6; // From 1.3 to 1.8
  } 

  std::size_t repetitions_over_array = total_size / L2_length; // Number of repetitions
  std::cout << "\n======= REPEAT = " << repetitions_over_array << "\n\n";

  auto func = [](auto in) { return in = in * 1.1; };
  auto func_eve = [](auto in) { return in = in * 1.1; };

  std::string l2_str = std::to_string(L2_length / kio);
  sutils::printer_t::head("Benchmark - transform, memory-bound (L2 " + l2_str + ")", true);

  transform_test<DATA_TYPE>( "transform memory-bound RAM"
                          , "transform_memory_RAM_" + kwk::bench::EVE_COMPILER_FLAG + "_L2-" + l2_str + ".bench"
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
};
#endif // ENABLE_RAM

#if ENABLE_L2
TTS_CASE("Benchmark - transform, memory-bound L2")
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

  std::size_t repetitions_over_array = total_size / L2_length; // Number of repetitions
  std::cout << "\n======= REPEAT = " << repetitions_over_array << "\n\n";

  auto func = [](auto in) { return in = in * 1.1; };
  auto func_eve = [](auto in) { return in = in * 1.1; };

  std::string l2_str = std::to_string(L2_length / kio);
  sutils::printer_t::head("Benchmark - transform, memory-bound (L2 " + l2_str + ")", true);

  transform_test<DATA_TYPE>( "transform memory-bound L2 cache"
                          , "transform_memory_L2_" + kwk::bench::EVE_COMPILER_FLAG + "_L2-" + l2_str + ".bench"
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
};
#endif // ENABLE_L2
#endif // ENABLE_MEMORY

#endif // KIWAKU_BUILD_BENCH