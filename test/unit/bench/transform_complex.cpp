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
void transform_test( std::string const& bench_name
                  , std::string const& file_name
                  , auto func_transform_re
                  , auto func_transform_im
                  , std::size_t const L2_size
                  , std::size_t const total_number_of_elements
                  , const data_reset_t data_reset
                  , std::size_t repeat_compute_intensity
                  )
{
  constexpr bool enable_check = ENABLE_CHECK;
  const std::size_t repeat_count = 1; // deprecated, led to erroneous benchmarks
  std::size_t total_data_size = total_number_of_elements * sizeof(DATA_TYPE) * 4;

  // std::cout << "L2_size(" << L2_size << ") repeat_count(" << repeat_count << ")\n";

  // The algorithm writes into out and reads from in.
  std::vector<DATA_TYPE> vector_re_in(L2_size);
  std::vector<DATA_TYPE> vector_im_in(L2_size);
  std::vector<DATA_TYPE> vector_re_out(L2_size);
  std::vector<DATA_TYPE> vector_im_out(L2_size);

  // Also justified for SYCL, since a single kernel cannot have simultaneous
  // read and write access to the same buffer through separate accessors.

  // Remember result for checking other values
  std::vector<DATA_TYPE> truth_re_out(L2_size);
  std::vector<DATA_TYPE> truth_im_out(L2_size);

  // Only used once now, since output and inputs are separated
  auto reset_data = [&](std::vector<DATA_TYPE>& re, std::vector<DATA_TYPE>& im)
  {
    if (data_reset == data_reset_t::ones)
    {
      std::fill(re.begin(), re.end(), 1);
      std::fill(im.begin(), im.end(), 1);
    }
    if (data_reset == data_reset_t::trigo) 
    {
      std::mt19937 gen(78512); // rd()
      std::uniform_real_distribution<float> dis(0, 2.0 * M_PI);
      for (std::size_t i = 0; i < L2_size; ++i)
      {
        float val = dis(gen);
        re[i] = std::sin(val);
        im[i] = std::cos(val);
      }
    }
  };

  reset_data(vector_re_in, vector_im_in);

  // Kiwaku views
  auto kwk_re_in   = kwk::view{kwk::source = vector_re_in.data()  , kwk::of_size(L2_size)};
  auto kwk_re_out  = kwk::view{kwk::source = vector_re_out.data() , kwk::of_size(L2_size)};
  auto kwk_im_in   = kwk::view{kwk::source = vector_im_in.data()  , kwk::of_size(L2_size)};
  auto kwk_im_out  = kwk::view{kwk::source = vector_im_out.data() , kwk::of_size(L2_size)};


  // Benchmark initialization
  kwk::bench::cbench_t b; // Processed elements, per second (higher is better)
  b.start(kwk::bench::fprefix() + file_name, bench_name, "Throughput", total_number_of_elements);
  // b.set_iterations(1);



  // // TODO: Allouer les tableaux in_re, inout_im, out_re
  // for (std::size_t i = 0; i < repeat_inner; ++i) {
  //   kwk::map(out_re, [](auto re, auto im) { return re * re - im * im; },
  //            in_re, inout_im);
  //   kwk::map(inout_im,
  //            [](auto re, auto im) { return re * im + im * re; },
  //            in_re, inout_im);
  //   std::swap(in_re, out_re);
  // }


  // ====== Generic std function, used later ======
  auto fct_std_transform = [&](auto const& policy)
  {
    for (std::size_t repeat_co = 0; repeat_co < repeat_compute_intensity; ++repeat_co)
    {
      // [&](auto const re, auto const im) { return func_transform_re(re, im); }
      // Is function call expensive?
      std::transform ( policy
                    , vector_re_out.begin()
                    , vector_re_out.end()
                    , vector_re_in.begin()
                    , vector_im_in.begin()
                    , func_transform_re
                    );
      std::transform ( policy
                    , vector_im_out.begin()
                    , vector_im_out.end()
                    , vector_re_in.begin()
                    , vector_im_in.begin()
                    , func_transform_im
                    );
    }
    return vector_re_out[L2_size / 2] + vector_im_out[L2_size / 2];
  };


  // ====== Generic std benchmark ======
  auto bench_std = [&](auto const& policy, std::string policy_name)
  {
    b.run_function_rpt_bwidth( policy_name
                      , repeat_count
                      , [&]{ return fct_std_transform(policy); }
                      , []{ }
                      , total_data_size
                      );
  };


  // ====== Generic Kiwaku benchmark ======
  // -> DATA_TYPE
  // Should theoretically work for both CPU sequential and SIMD
  // (not for SYCL however)
  auto bench_kiwaku = [&](auto&& context, std::string context_name)
  {
    DATA_TYPE return_;
    auto fct_generic = [&]()
    {
      for (std::size_t repeat_co = 0; repeat_co < repeat_compute_intensity; ++repeat_co)
      {
        kwk::transform(context, func_transform_re, kwk_re_out, kwk_re_in, kwk_im_in);
        kwk::transform(context, func_transform_im, kwk_im_out, kwk_re_in, kwk_im_in);
      }
      return_ = kwk_re_out(L2_size / 2) + kwk_im_out(L2_size / 2);
      return return_;
    };
    b.run_function_rpt_bwidth ( context_name
                              , repeat_count
                              , fct_generic
                              , []{ }
                              , total_data_size
                              );
  };


  // ====== hand-written sequential ======
  auto fct_hand = [&]()
  {
    for (std::size_t repeat_co = 0; repeat_co < repeat_compute_intensity; ++repeat_co)
    {
      for (std::size_t i = 0; i < L2_size; ++i)
      {
        vector_re_out[i] = func_transform_re(vector_re_in[i], vector_im_in[i]);
        vector_im_out[i] = func_transform_im(vector_re_in[i], vector_im_in[i]);
      }
    }
    return vector_re_out[L2_size / 2] + vector_im_out[L2_size / 2];
  };
  b.run_function_rpt_bwidth("hand CPU", repeat_count, fct_hand, []{ }, total_data_size);

  // Remember output for later verification
  std::copy(vector_re_out.begin(), vector_re_out.end(), truth_re_out.begin());
  std::copy(vector_im_out.begin(), vector_im_out.end(), truth_im_out.begin());


  // ====== Kiwaku CPU context ======
  bench_kiwaku(::kwk::cpu, "kwk CPU");
  if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_re_out, truth_re_out, 1);
  if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_im_out, truth_im_out, 1);


  // ====== Kiwaku SIMD ======
  #if KIWAKU_BENCH_EVE
    bench_kiwaku(::kwk::simd, "kwk SIMD");
    if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_re_out, truth_re_out, 1);
    if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_im_out, truth_im_out, 1);
  #endif


  // ====== std sequential ======
  bench_std(std::execution::seq, "seq");
  if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_re_out, truth_re_out, 1);
  if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_im_out, truth_im_out, 1);


  // ====== std unsequenced ======
  bench_std(std::execution::unseq, "unseq");
  if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_re_out, truth_re_out, 1);
  if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_im_out, truth_im_out, 1);


  // Don't forget the -ltbb compiler flag
  #if ENABLE_TBB && KIWAKU_BENCH_MTHREAD

    // ====== std parallel ======
    bench_std(std::execution::par, "par");
    if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_re_out, truth_re_out, 1);
    if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_im_out, truth_im_out, 1);

    // ====== std parallel unsequenced ======
    bench_std(std::execution::par_unseq, "par_unseq");
    if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_re_out, truth_re_out, 1);
    if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_im_out, truth_im_out, 1);
  #endif



  // ====== SYCL ======
  #if KIWAKU_BENCH_SYCL
  
    // OSEF pour le manuscrit
    // #if KIWAKU_BENCH_MTHREAD
    //   // ====== CPU ======
    //   auto& ctx = kwk::sycl::default_context;
    //   bench_kiwaku(ctx, "kwk SYCL " + ctx.get_device_name(), map_func);
    //   if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_re_out, truth_re_out, 1);
    //   if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_im_out, truth_im_out, 1);
    // #endif

    // Don't forget -fsycl-targets=nvptx64-nvidia-cuda,x86_64  (with x86_64 or spir64)
    bool has_gpu = kwk::sycl::has_gpu();

    // Execute SYCL benchmark on GPU and CPU
    if (has_gpu)
    {
      // ====== Kiwaku SYCL GPU ======
      auto ctx_gpu = kwk::sycl::context{::sycl::gpu_selector_v};

      // Plusieurs runs pour vérifier le temps initial
      for (std::size_t run = 0; run < 2; ++run)
      {
        {
          std::string run_str = "run(" + std::to_string(run) + ")";
          std::string context_name = run_str + " kwk SYCL " + ctx_gpu.get_device_name();
          // Only pays the 1st data transfer
          auto proxy_re_out = ctx_gpu.out(kwk_re_out);
          auto proxy_im_out = ctx_gpu.out(kwk_im_out);
          auto proxy_re_in  = ctx_gpu.in(kwk_re_in);
          auto proxy_im_in  = ctx_gpu.in(kwk_im_in);

          auto fct_transforms = [&]()
          {
            for (std::size_t repeat_co = 0; repeat_co < repeat_compute_intensity; ++repeat_co)
            {
              kwk::transform_proxy(ctx_gpu, func_transform_re, proxy_re_out, proxy_re_in, proxy_im_in);
              kwk::transform_proxy(ctx_gpu, func_transform_im, proxy_im_out, proxy_re_in, proxy_im_in);
            }
            return kwk_re_out(L2_size / 2) + kwk_im_out(L2_size / 2);
          };

          b.run_function_rpt_bwidth ( context_name
                                    , repeat_count
                                    , fct_transforms
                                    , []{ }
                                    , total_data_size
                                    );
        } // Kiwaku proxy out of scope = SYCL buffer destruction = data back to host
        if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_re_out, truth_re_out, 1);
        if constexpr(enable_check) TTS_ALL_RELATIVE_EQUAL(vector_im_out, truth_im_out, 1);
      }

    }
  #endif



  for (std::size_t i = 0; i < 30; ++i) { std::cout << truth_re_out[i] << " "; }
  std::cout << "\n";
  for (std::size_t i = 0; i < 30; ++i) { std::cout << truth_im_out[i] << " "; }
  std::cout << "\n";
  b.stop();
}



TTS_CASE("Benchmark - transform, compute-bound")
{
  ::kwk::bench::get_eve_compiler_flag();

  using DATA_TYPE = float;

  // const DATA_TYPE PI = static_cast<DATA_TYPE>(M_PI);



  [[maybe_unused]] std::size_t kio = 1024 / (sizeof(DATA_TYPE) * 1); // input + output
  [[maybe_unused]] std::size_t mio = 1024 * kio;
  [[maybe_unused]] std::size_t gio = 1024 * mio;

  std::size_t total_size;
  std::size_t L2_size; // L2 cache size, in bytes
  std::string hname = sutils::get_host_name();
  // Total data to process
        if (hname == "parsys-legend")          { total_size = 1 * gio * kwk::bench::LEGEND_LOAD_FACTOR; L2_size = total_size; } 
        // if (hname == "parsys-legend")          { L2_size = 512 * kio; total_size = 1 * gio * kwk::bench::LEGEND_LOAD_FACTOR; } 

  else if (hname == "pata")                   { total_size = 128 * mio; L2_size = total_size; }
  else if (hname == "chaton")                 { total_size = 32 * mio; L2_size = total_size; }
  // else if (hname == "chaton")                 { L2_size = 256 * kio; total_size = 128 * mio; }
  else if (hname == "sylvain-ThinkPad-T580")  { L2_size = 256 * kio; total_size = 8 * mio; }

  // 512 kio par L2
  else if (hname == "falcou-avx512")          { total_size = 512 * mio;  L2_size = total_size; //128 * kio; 
                                                std::cout << "OK, Used computer: falcou-avx512\n"; }

  // La Pierre
  else if (hname == "lapierre")               { L2_size = 128 * mio; total_size = 256 * mio; }
  else                                        { L2_size = 256 * kio; total_size = 8 * mio; }

  std::size_t repetitions = total_size / L2_size; // Number of repetitions

  std::cout << "\n======= REPEAT = " << repetitions << "\n\n";


  // === Unitary complex ===


  // const std::size_t repeat_inner = 10;
  // auto map_func = [=](DATA_TYPE& re, DATA_TYPE& im)
  // {
    // for (std::size_t i = 0; i < repeat_inner; ++i) {
      // DATA_TYPE re_ = re;
      // DATA_TYPE im_ = im;
      // re = re_ * re_ - im_ * im_;
      // im = re_ * im_ + im_ * re_;
    // }
  // };
  // auto map_func_eve = map_func;




  // // TODO: Allouer les tableaux in_re, inout_im, out_re
  // for (std::size_t i = 0; i < repeat_inner; ++i) {
  //   kwk::map(out_re, [](auto re, auto im) { return re * re - im * im; },
  //            in_re, inout_im);
  //   kwk::map(inout_im,
  //            [](auto re, auto im) { return re * im + im * re; },
  //            in_re, inout_im);
  //   std::swap(in_re, out_re);
  // }

  // kwk::transform(::kwk::simd, map_func_, kwk_re, kwk_im);
  // auto map_func_eve = [=](auto& re, auto& im)
  // {
    
  //   re = ::eve::cos(re) * ::eve::cos(re) + ::eve::sin(re) * ::eve::sin(re);
  //   im = ::eve::cos(im) * ::eve::cos(im) + ::eve::sin(im) * ::eve::sin(im);
  // };


  // kwk::transform(::kwk::simd, map_func_, kwk_re, kwk_im);
  // auto map_func_eve = [=](auto& re, auto& im)
  // {
  //   for (std::size_t r = 0; r < repetitions; ++r)
  //   {
  //     re = ::eve::cos(re) * ::eve::cos(re) + ::eve::sin(re) * ::eve::sin(re);
  //     im = ::eve::cos(im) * ::eve::cos(im) + ::eve::sin(im) * ::eve::sin(im);
  //   }
  // };


  auto func_transform1 = [](auto re, auto im) { return re * re - im * im; };
  auto func_transform2 = [](auto re, auto im) { return re * im + im * re; };



  std::string l2_str = std::to_string(L2_size / kio);

  std::size_t compute_intensity = 10;


  sutils::printer_t::head("Benchmark - transform, compute-bound (L2 " + l2_str + ")", true);

  transform_test<DATA_TYPE>( "transform compute-bound"
                          , "transform_compute-bound_" + kwk::bench::EVE_COMPILER_FLAG + "_L2-" + l2_str + ".bench"
                          , func_transform1
                          , func_transform2
                          , L2_size
                          , L2_size * repetitions
                          , data_reset_t::trigo
                          , compute_intensity
                          );
  std::cout << "\n\n";
};


// TTS_CASE("Benchmark - transform, memory-bound")
// {
//   ::kwk::bench::get_eve_compiler_flag();

//   using DATA_TYPE = float;

//   auto map_func = [=](DATA_TYPE& re, DATA_TYPE& im)
//   {
//     re = re + 2;
//     im = im + 2;
//   };

//   auto map_func_eve = [=](DATA_TYPE& re, DATA_TYPE& im)
//   {
//     re = re + 2;
//     im = im + 2;
//   };

//   [[maybe_unused]] std::size_t kio = 1024 / (sizeof(DATA_TYPE) * 1); // Only one item for (read + (read + write))
//   [[maybe_unused]] std::size_t mio = 1024 * kio;
//   [[maybe_unused]] std::size_t gio = 1024 * mio;

//   std::size_t total_size;
//   std::size_t L2_size; // L2 cache size, in bytes
//   std::string hname = sutils::get_host_name();
//   // Total data to process
//         if (hname == "parsys-legend")          { L2_size = 512 * kio; total_size = 1 * gio * kwk::bench::LEGEND_LOAD_FACTOR; } 
//   else if (hname == "pata")                   { total_size = 2 * gio; L2_size = total_size; }
//   else if (hname == "chaton")                 { total_size = 2 * gio; L2_size = total_size; }
//   else if (hname == "sylvain-ThinkPad-T580")  { L2_size = 256 * kio; total_size = 8 * mio; }

//   // 512 kio par L2
//   else if (hname == "falcou-avx512")          { total_size = 2 * gio; L2_size = total_size; // memory-bound
//                                                 std::cout << "OK, Used computer: falcou-avx512\n"; }

//   // La Pierre
//   else if (hname == "lapierre")               { L2_size = 128 * mio; total_size = 256 * mio; }
//   else                                        { L2_size = 256 * kio; total_size = 8 * mio; }

//   std::string l2_str = std::to_string(L2_size / kio);

//   sutils::printer_t::head("Benchmark - transform, memory-bound (L2 " + l2_str + ")", true);

//   transform_test<DATA_TYPE>( "transform memory-bound"
//                           , "transform_memory-bound_" + kwk::bench::EVE_COMPILER_FLAG + "_L2-" + l2_str + ".bench"
//                           , map_func
//                           , map_func_eve
//                           , L2_size
//                           , L2_size * 1
//                           , data_reset_t::ones
//                           );
//   std::cout << "\n\n";
// };


#endif // KIWAKU_BUILD_BENCH