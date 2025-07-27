//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <kwk/context/sycl/context.hpp>
#include <kwk/context/cpu/context.hpp>

// For each SYCL
// ninja unit.algorithm.algos.context.sycl.outside_access.exe && ./unit/unit.algorithm.algos.context.sycl.outside_access.exe


// Uses a Kiwaku container inside a SYCL kernel

// Reference implementation on CPU
TTS_CASE("Check for kwk::for_each(kwk::cpu, func, container) 1D")
{
  using data_type = int;
  const std::size_t size = 20;
  std::array<data_type, size> input;
  std::array<data_type, size> output;
  std::array<data_type, size> expected_output;

  // auto fn = [](data_type const e) { return e * 3 + 5; };

  for (std::size_t i = 0; i < size; ++i)
  {
    input[i] = i;
    output[i] = 0;
    expected_output[i] = input[i] * 3 + 5;
  }

  auto view_input  = kwk::view{kwk::source = input , kwk::of_size(size)};
  auto view_output = kwk::view{kwk::source = output, kwk::of_size(size)};

  auto context = kwk::sycl::default_context;


  // kwk::sycl::default_context.prepare_kernel = [&](::sycl::handler& h)
  // {
  //   auto proxy = context.inout(view_output);
  //   auto acc   = proxy.access(h);

  // };

  // // auto prepare_kernel = [&](::sycl::handler& h)
  // // {
  // //   auto proxy = context.inout(view_output);
  // //   auto acc   = proxy.access(h);
  // //   std::cout << "Prepare kernel called!\n";

  // // };

  // // auto kernel = [&](auto& e1, auto& e2) 
  // // {
  // //   e2 = e1 * 3 + 5;
  // // };


  // // auto call_all = [&]()
  // // {
  // //   prepare_kernel(h);

  // //   kernel(e1, e2);
  // // };

  auto kernel = [&](auto i, auto& acc1, auto& acc2) 
  {
    acc2[i] = acc1[i] * 3 + 5;
  };


  context.map_ext(kernel, context.in(view_input), context.out(view_output));

  // cannot be used as the type of a kernel parameter
  // kwk::for_each(context, kernel, view_input, view_output);
  // kwk::for_each(kwk::cpu, kernel, view_input);

  TTS_ALL_EQUAL(expected_output, output);
  // TTS_RELATIVE_EQUAL(total, chk_total, FLOAT_TOLERANCE_PERCENT);
};

// clear && ninja unit.algorithm.algos.context.sycl.for_each.exe && ./unit/unit.algorithm.algos.context.sycl.for_each.exe
// clear && ninja unit.algorithm.algos.context.sycl.outside_access.exe && ./unit/unit.algorithm.algos.context.sycl.outside_access.exe























// // Reference implementation on CPU
// TTS_CASE("Check for kwk::for_each(kwk::cpu, func, container) 1D")
// {
//   using data_type = int;
//   const std::size_t size = 20;
//   std::array<data_type, size> input;
//   std::array<data_type, size> output;
//   std::array<data_type, size> expected_output;

//   // auto fn = [](data_type const e) { return e * 3 + 5; };

//   for (std::size_t i = 0; i < size; ++i)
//   {
//     input[i] = i;
//     output[i] = 0;
//     expected_output[i] = input[i] * 3 + 5;
//   }

//   auto view_input  = kwk::view{kwk::source = input , kwk::of_size(size)};
//   auto view_output = kwk::view{kwk::source = output, kwk::of_size(size)};

//   // data_type chk_total{0};
//   // for (data_type e : input) { chk_total += e; }

//   // std::size_t count{0};
//   // data_type total{0};

//   // kwk::for_each(wk::sycl::default_context, [&](auto const& e1, auto& e2) { e2 = e1 * 3 + 5; }, view_input, view_output);



//   auto context = kwk::sycl::default_context;

//   kwk::sycl::default_context.prepare_kernel = [&](::sycl::handler& h)
//   {
//     [[maybe_unused]] auto proxy = context.inout(view_output);
//     [[maybe_unused]] auto acc   = proxy.access(h);



//     std::cout << "Prepare kernel called!\n";
//   };

//   auto kernel = [&](::sycl::handler& h, auto& e1, auto& e2) 
//   {
//     e2 = e1 * 3 + 5;
//     // view_output(e1) = e1 * 3 + 5;
//   };

//   // cannot be used as the type of a kernel parameter
//   kwk::for_each(context, kernel, view_input, view_output);
//   // kwk::for_each(kwk::cpu, kernel, view_input);

//   TTS_ALL_EQUAL(expected_output, output);
//   // TTS_RELATIVE_EQUAL(total, chk_total, FLOAT_TOLERANCE_PERCENT);
// };