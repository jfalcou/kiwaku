//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#include <kwk/context/sycl/context.hpp>
#include <cstdlib>
#include <kwk/algorithm/algos/find.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <optional>

TTS_CASE("Check for kwk::find(kwk::sycl::default_context, In, value) 1D")
{
  // Empty array
  {
    const std::size_t input_size = 0;
    std::vector<int> input(input_size);
    auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};
    auto res  = kwk::find(kwk::sycl::default_context, view, 0);
    TTS_EQUAL(res.has_value(), false);
  }


  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 2 - 10; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  // Not found
  {
    auto res = kwk::find(kwk::sycl::default_context, view, 11);
    TTS_EQUAL(res.has_value(), false);
  }

  // First position
  {
    auto res = kwk::find(kwk::sycl::default_context, view, -10);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{0});
    else                 TTS_FAIL("find returned std::nullopt and not the expected valid value.");
  }

  // Random position
  {
    auto res = kwk::find(kwk::sycl::default_context, view, 12);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{11});
    else                 TTS_FAIL("find returned std::nullopt and not the expected valid value.");
  }

  // Last position
  {
    auto res = kwk::find(kwk::sycl::default_context, view, (static_cast<int>(input_size) - 1) * 2 - 10);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{input_size-1});
    else                 TTS_FAIL("find returned std::nullopt and not the expected valid value.");
  }
};


TTS_CASE("Check for kwk::find_if(kwk::sycl::default_context, In, func) 1D")
{
  // Empty array
  {
    const std::size_t input_size = 0;
    std::vector<int> input(input_size);
    auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};
    auto func = [&](auto item) { return (item % 2) == 0; };
    auto res  = kwk::find_if(kwk::sycl::default_context, view, func);
    TTS_EQUAL(res.has_value(), false);
  }

  // Array of size 20
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 2; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  auto func_even = [&](auto item) { return (item % 2) == 0; };
  auto func_odd = [&](auto item) { return (item % 2) == 1; };

  // First element should be valid
  {
    auto res = kwk::find_if(kwk::sycl::default_context, view, func_even);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{0});
    else                 TTS_FAIL("find_if returned std::nullopt and not the expected valid value.");
  }

  // Odd item not found
  {
    auto res = kwk::find_if(kwk::sycl::default_context, view, func_odd);
    TTS_EQUAL(res.has_value(), false);
  }

  // Added matching item (odd number) at random position : 12
  input[12] = 7;
  {
    auto res = kwk::find_if(kwk::sycl::default_context, view, func_odd);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{12});
    else                 TTS_FAIL("find_if returned std::nullopt and not the expected valid value.");
  }

  // Test first position
  input[12] = 24;
  input[0] = 7;
  {
    auto res = kwk::find_if(kwk::sycl::default_context, view, func_odd);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{0});
    else                 TTS_FAIL("find_if returned std::nullopt and not the expected valid value.");
  }

  // Test last position
  input[0] = 0;
  input[input_size-1] = 7;
  {
    auto res = kwk::find_if(kwk::sycl::default_context, view, func_odd);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{input_size-1});
    else                 TTS_FAIL("find_if returned std::nullopt and not the expected valid value.");
  }
};

TTS_CASE("Check for kwk::find_if_not(kwk::sycl::default_context, In, func) 1D")
{
  // Empty array
  {
    const std::size_t input_size = 0;
    std::vector<int> input(input_size);
    auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

    auto func = [&](auto item) { return (item % 2) == 0; };
    auto res = kwk::find_if_not(kwk::sycl::default_context, view, func);

    TTS_EQUAL(res.has_value(), false);
  }

  // Non-empty array
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 2; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  // Looking for the first item that is not odd
  {
    auto func_odd = [&](auto item) { return (item % 2) == 1; };
    auto res = kwk::find_if_not(kwk::sycl::default_context, view, func_odd);

    // To avoid a nasty compiler error
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{0});
    else                 TTS_FAIL("find_if_not returned std::nullopt and not the expected valid value.");
  }

  // Not found case: looking for the first item that is not even
  auto func_even = [&](auto item) { return (item % 2) == 0; };
  {
    // "not even" item not found
    auto res = kwk::find_if_not(kwk::sycl::default_context, view, func_even);
    TTS_EQUAL(res.has_value(), false);
  }

  // Added matching item (odd number) at a random position
  input[12] = 7;
  {
    auto res = kwk::find_if_not(kwk::sycl::default_context, view, func_even);
    // To avoid a nasty compiler error
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{12});
    else                 TTS_FAIL("find_if_not returned std::nullopt and not the expected valid value.");
  }

  // Test first position
  input[12] = 24;
  input[0] = 3;
  {
    auto res = kwk::find_if_not(kwk::sycl::default_context, view, func_even);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{0});
    else                 TTS_FAIL("find_if_not returned std::nullopt and not the expected valid value.");
  }

  // Test last position
  input[0] = 0;
  input[input_size-1] = 3;
  {
    auto res = kwk::find_if_not(kwk::sycl::default_context, view, func_even);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{input_size-1});
    else                 TTS_FAIL("find_if_not returned std::nullopt and not the expected valid value.");
  }
};


// TODO: SYCL issue with nested kernels.



// TTS_CASE("Check for kwk::find_last(kwk::sycl::default_context, In, value) 1D")
// {
//   // // Empty array
//   {
//     const std::size_t input_size = 0;
//     std::vector<int> input(input_size);
//     auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};
//     int search{6};
//     auto res = kwk::find_last(kwk::sycl::default_context, view, search);
//     TTS_EQUAL(res.has_value(), false);
//   }

//   const std::size_t input_size = 40;
//   std::array<int, input_size> input;
//   for (std::size_t i = 0; i < input_size; ++i) { input[i] = 6; }
//   auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

//   // Not found
//   {
//     auto res = kwk::find_last(kwk::sycl::default_context, view, 8);
//     TTS_EQUAL(res.has_value(), false);
//   }
  
//   // Last position
//   {
//     auto res = kwk::find_last(kwk::sycl::default_context, view, 6);
//     if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{input_size-1});
//     else                 TTS_FAIL("find_last returned std::nullopt and not the expected valid value.");
//   }

//   // Random position
//   input[4] = 8;
//   {
//     auto res = kwk::find_last(kwk::sycl::default_context, view, 8);
//     if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{4});
//     else                 TTS_FAIL("find_last returned std::nullopt and not the expected valid value.");
//   }
//   input[4] = 6;

//   // First position
//   input[0] = 8;
//   {
//     auto res = kwk::find_last(kwk::sycl::default_context, view, 8);
//     if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{0});
//     else                 TTS_FAIL("find_last returned std::nullopt and not the expected valid value.");
//   }
// };





// ================================ TODO ================================
// TODO: fix issues here, compiles but returns bad results.
// TTS_CASE("Check for kwk::find_last_if(kwk::sycl::default_context, In, func) 1D")
// {
//   auto func = [](auto const& e) { return (e % 2) == 0; };

//   // Empty array
//   {
//     const std::size_t input_size = 0;
//     std::vector<int> input(input_size);
//     auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};
//     auto res = kwk::find_last_if(kwk::sycl::default_context, view, func);
//     TTS_EQUAL(res.has_value(), false);
//   }

//   const std::size_t input_size = 40;
//   std::array<int, input_size> input;
//   for (std::size_t i = 0; i < input_size; ++i) { input[i] = 5; }
//   auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

//   // Not found
//   {
//     auto res = kwk::find_last_if(kwk::sycl::default_context, view, func);
//     TTS_EQUAL(res.has_value(), false);
//   }

//   // Last position
//   input[input_size-1] = 8;
//   {
//     auto res = kwk::find_last_if(kwk::sycl::default_context, view, func);
//     if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{input_size-1});
//     else                 TTS_FAIL("find_last_if returned std::nullopt and not the expected valid value.");
//   }
//   input[input_size-1] = 5;

//   // Random position
//   input[4] = 16;
//   {
//     auto res = kwk::find_last_if(kwk::sycl::default_context, view, func);
//     if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{4});
//     else                 TTS_FAIL("find_last_if returned std::nullopt and not the expected valid value.");
//   }
//   input[4] = 5;

//   // First position
//   input[0] = 6;
//   {
//     auto res = kwk::find_last_if(kwk::sycl::default_context, view, func);
//     if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{0});
//     else                 TTS_FAIL("find_last_if returned std::nullopt and not the expected valid value.");
//   }
// };

// TTS_CASE("Check for kwk::find_last_if_not(kwk::sycl::default_context, In, func) 1D")
// {

//   auto func = [](auto const& e) { return (e % 2) != 0; };

//   // Empty array
//   {
//     const std::size_t input_size = 0;
//     std::vector<int> input(input_size);
//     auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};
//     auto res = kwk::find_last_if_not(kwk::sycl::default_context, view, func);
//     TTS_EQUAL(res.has_value(), false);
//   }

//   const std::size_t input_size = 40;
//   std::array<int, input_size> input;
//   for (std::size_t i = 0; i < input_size; ++i) { input[i] = 5; }
//   auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

//   // Not found
//   {
//     auto res = kwk::find_last_if_not(kwk::sycl::default_context, view, func);
//     TTS_EQUAL(res.has_value(), false);
//   }

//   // Last position
//   input[input_size-1] = 8;
//   {
//     auto res = kwk::find_last_if_not(kwk::sycl::default_context, view, func);
//     if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{input_size-1});
//     else                 TTS_FAIL("find_last_if_not returned std::nullopt and not the expected valid value.");
//   }
//   input[input_size-1] = 5;

//   // Random position
//   input[4] = 16;
//   {
//     auto res = kwk::find_last_if_not(kwk::sycl::default_context, view, func);
//     if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{4});
//     else                 TTS_FAIL("find_last_if_not returned std::nullopt and not the expected valid value.");
//   }
//   input[4] = 5;

//   // First position
//   input[0] = 6;
//   {
//     auto res = kwk::find_last_if_not(kwk::sycl::default_context, view, func);
//     if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{0});
//     else                 TTS_FAIL("find_last_if_not returned std::nullopt and not the expected valid value.");
//   }
// };
