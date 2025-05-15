//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#include <cstdlib>
#include <kwk/algorithm/algos/find.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <optional>
#include <kwk/context/cpu/context.hpp>

TTS_CASE("Check for kwk::find(kwk::cpu, In, value) 1D")
{
  // Empty array
  {
    const std::size_t input_size = 0;
    std::vector<int> input(input_size);
    auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};
    auto res  = kwk::find(kwk::cpu, view, 0);
    TTS_EQUAL(res.has_value(), false);
  }

  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 2 - 10; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  // Not found
  {
    auto res = kwk::find(kwk::cpu, view, 11);
    TTS_EQUAL(res.has_value(), false);
  }

  // First position
  {
    auto res = kwk::find(kwk::cpu, view, -10);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{0});
    else                 TTS_FAIL("find returned std::nullopt and not the expected valid value.");
  }

  // Random position
  {
    auto res = kwk::find(kwk::cpu, view, 12);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{11});
    else                 TTS_FAIL("find returned std::nullopt and not the expected valid value.");
  }

  // Last position
  {
    auto res = kwk::find(kwk::cpu, view, (static_cast<int>(input_size) - 1) * 2 - 10);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{input_size-1});
    else                 TTS_FAIL("find returned std::nullopt and not the expected valid value.");
  }
};


TTS_CASE("Check for kwk::find_if(kwk::cpu, In, func) 1D")
{
  // Empty array
  {
    const std::size_t input_size = 0;
    std::vector<int> input(input_size);
    auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};
    auto func = [&](auto item) { return (item % 2) == 0; };
    auto res  = kwk::find_if(kwk::cpu, view, func);
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
    auto res = kwk::find_if(kwk::cpu, view, func_even);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{0});
    else                 TTS_FAIL("find_if returned std::nullopt and not the expected valid value.");
  }

  // Odd item not found
  {
    auto res = kwk::find_if(kwk::cpu, view, func_odd);
    TTS_EQUAL(res.has_value(), false);
  }

  // Added matching item (odd number) at random position : 12
  input[12] = 7;
  {
    auto res = kwk::find_if(kwk::cpu, view, func_odd);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{12});
    else                 TTS_FAIL("find_if returned std::nullopt and not the expected valid value.");
  }

  // Test first position
  input[12] = 24;
  input[0] = 7;
  {
    auto res = kwk::find_if(kwk::cpu, view, func_odd);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{0});
    else                 TTS_FAIL("find_if returned std::nullopt and not the expected valid value.");
  }

  // Test last position
  input[0] = 0;
  input[input_size-1] = 7;
  {
    auto res = kwk::find_if(kwk::cpu, view, func_odd);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{input_size-1});
    else                 TTS_FAIL("find_if returned std::nullopt and not the expected valid value.");
  }
};

TTS_CASE("Check for kwk::find_if_not(kwk::cpu, In, func) 1D")
{
  // Empty array
  {
    const std::size_t input_size = 0;
    std::vector<int> input(input_size);
    auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

    auto func = [&](auto item) { return (item % 2) == 0; };
    auto res = kwk::find_if_not(kwk::cpu, view, func);

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
    auto res = kwk::find_if_not(kwk::cpu, view, func_odd);

    // To avoid a nasty compiler error
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{0});
    else                 TTS_FAIL("find_if_not returned std::nullopt and not the expected valid value.");
  }

  // Not found case: looking for the first item that is not even
  auto func_even = [&](auto item) { return (item % 2) == 0; };
  {
    // "not even" item not found
    auto res = kwk::find_if_not(kwk::cpu, view, func_even);
    TTS_EQUAL(res.has_value(), false);
  }

  // Added matching item (odd number) at a random position
  input[12] = 7;
  {
    auto res = kwk::find_if_not(kwk::cpu, view, func_even);
    // To avoid a nasty compiler error
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{12});
    else                 TTS_FAIL("find_if_not returned std::nullopt and not the expected valid value.");
  }

  // Test first position
  input[12] = 24;
  input[0] = 3;
  {
    auto res = kwk::find_if_not(kwk::cpu, view, func_even);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{0});
    else                 TTS_FAIL("find_if_not returned std::nullopt and not the expected valid value.");
  }

  // Test last position
  input[0] = 0;
  input[input_size-1] = 3;
  {
    auto res = kwk::find_if_not(kwk::cpu, view, func_even);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{input_size-1});
    else                 TTS_FAIL("find_if_not returned std::nullopt and not the expected valid value.");
  }
};


// TODO?: Implement find_last_of?
TTS_CASE("Check for kwk::find_first_of(kwk::cpu, In, In) 1D")
{
  // Empty array
  {
    const std::size_t input_size = 0;
    std::vector<int> input(input_size);
    auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};
    std::vector<int> search{-10, -5, 0, 8, 11};
    auto sv = kwk::view{kwk::source = search, kwk::of_size(search.size())};
    auto res = kwk::find_first_of(kwk::cpu, view, sv);
    TTS_EQUAL(res.has_value(), false);
  }

  const std::size_t input_size = 40;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 2; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  // No element found
  {
    std::vector<int> search{-10, -5, 1, 7, 11};
    auto sv  = kwk::view{kwk::source = search, kwk::of_size(search.size())};
    auto res = kwk::find_first_of(kwk::cpu, view, sv);
    TTS_EQUAL(res.has_value(), false);
  }

  // Element "0" found at position 0
  {
    std::vector<int> search{-10, -5, 0, 8, 11};
    auto sv  = kwk::view{kwk::source = search, kwk::of_size(search.size())};
    auto res = kwk::find_first_of(kwk::cpu, view, sv);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{0});
    else                 TTS_FAIL("find_first_of returned std::nullopt and not the expected valid value.");
  }

  // Element "8" found at position 4
  {
    std::vector<int> search{-10, -5, 1, 16, 8};
    auto sv  = kwk::view{kwk::source = search, kwk::of_size(search.size())};
    auto res = kwk::find_first_of(kwk::cpu, view, sv);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{4});
    else                 TTS_FAIL("find_first_of returned std::nullopt and not the expected valid value.");
  }

  // Element "input_size*2-2" found at last position: input_size-1
  {
    std::vector<int> search{input_size*2-2, -5, 1, 7, 11};
    auto sv  = kwk::view{kwk::source = search, kwk::of_size(search.size())};
    auto res = kwk::find_first_of(kwk::cpu, view, sv);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{input_size-1});
    else                 TTS_FAIL("find_first_of returned std::nullopt and not the expected valid value.");
  }
};


TTS_CASE("Check for kwk::find_last(kwk::cpu, In, value) 1D")
{
  // // Empty array
  {
    const std::size_t input_size = 0;
    std::vector<int> input(input_size);
    auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};
    int search{6};
    auto res = kwk::find_last(kwk::cpu, view, search);
    TTS_EQUAL(res.has_value(), false);
  }

  const std::size_t input_size = 40;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = 6; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  // Not found
  {
    auto res = kwk::find_last(kwk::cpu, view, 8);
    TTS_EQUAL(res.has_value(), false);
  }
  
  // Last position
  {
    auto res = kwk::find_last(kwk::cpu, view, 6);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{input_size-1});
    else                 TTS_FAIL("find_last returned std::nullopt and not the expected valid value.");
  }

  // Random position
  input[4] = 8;
  {
    auto res = kwk::find_last(kwk::cpu, view, 8);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{4});
    else                 TTS_FAIL("find_last returned std::nullopt and not the expected valid value.");
  }
  input[4] = 6;

  // First position
  input[0] = 8;
  {
    auto res = kwk::find_last(kwk::cpu, view, 8);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{0});
    else                 TTS_FAIL("find_last returned std::nullopt and not the expected valid value.");
  }
};

TTS_CASE("Check for kwk::find_last_if(kwk::cpu, In, func) 1D")
{
  auto func = [](auto const& e) { return (e % 2) == 0; };

  // Empty array
  {
    const std::size_t input_size = 0;
    std::vector<int> input(input_size);
    auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};
    auto res = kwk::find_last_if(kwk::cpu, view, func);
    TTS_EQUAL(res.has_value(), false);
  }

  const std::size_t input_size = 40;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = 5; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  // Not found
  {
    auto res = kwk::find_last_if(kwk::cpu, view, func);
    TTS_EQUAL(res.has_value(), false);
  }

  // Last position
  input[input_size-1] = 8;
  {
    auto res = kwk::find_last_if(kwk::cpu, view, func);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{input_size-1});
    else                 TTS_FAIL("find_last_if returned std::nullopt and not the expected valid value.");
  }
  input[input_size-1] = 5;

  // Random position
  input[4] = 16;
  {
    auto res = kwk::find_last_if(kwk::cpu, view, func);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{4});
    else                 TTS_FAIL("find_last_if returned std::nullopt and not the expected valid value.");
  }
  input[4] = 5;

  // First position
  input[0] = 6;
  {
    auto res = kwk::find_last_if(kwk::cpu, view, func);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{0});
    else                 TTS_FAIL("find_last_if returned std::nullopt and not the expected valid value.");
  }
};

TTS_CASE("Check for kwk::find_last_if_not(kwk::cpu, In, func) 1D")
{

  auto func = [](auto const& e) { return (e % 2) != 0; };

  // Empty array
  {
    const std::size_t input_size = 0;
    std::vector<int> input(input_size);
    auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};
    auto res = kwk::find_last_if_not(kwk::cpu, view, func);
    TTS_EQUAL(res.has_value(), false);
  }

  const std::size_t input_size = 40;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = 5; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  // Not found
  {
    auto res = kwk::find_last_if_not(kwk::cpu, view, func);
    TTS_EQUAL(res.has_value(), false);
  }

  // Last position
  input[input_size-1] = 8;
  {
    auto res = kwk::find_last_if_not(kwk::cpu, view, func);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{input_size-1});
    else                 TTS_FAIL("find_last_if_not returned std::nullopt and not the expected valid value.");
  }
  input[input_size-1] = 5;

  // Random position
  input[4] = 16;
  {
    auto res = kwk::find_last_if_not(kwk::cpu, view, func);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{4});
    else                 TTS_FAIL("find_last_if_not returned std::nullopt and not the expected valid value.");
  }
  input[4] = 5;

  // First position
  input[0] = 6;
  {
    auto res = kwk::find_last_if_not(kwk::cpu, view, func);
    if (res.has_value()) TTS_EQUAL(res.value(), kwk::position<1>{0});
    else                 TTS_FAIL("find_last_if_not returned std::nullopt and not the expected valid value.");
  }
};
