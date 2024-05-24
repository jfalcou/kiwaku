//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <cstdlib>
#include <kwk/algorithm/algos/find.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

TTS_CASE("Check for kwk::find(In, value) 1D")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 2; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  // std::cout << "find -1: " << kwk::find(view, -1) << "\n";
  // std::cout << "find -10: " << kwk::find(view, -10) << "\n";
  // std::cout << "find 0: " << kwk::find(view, 0) << "\n";
  // std::cout << "find 10: " << kwk::find(view, 10) << "\n";
  // std::cout << "find 100: " << kwk::find(view, 100) << "\n";

  {
    int search = -10;
    kumi::tuple<int> expected{-1};
    // kumi::tuple<int> expected = kumi::make_tuple<int>(-1);
    TTS_EQUAL(kwk::find(view, search), expected);
  }
  {
    int search = -1;
    kumi::tuple<int> expected{-1};
    TTS_EQUAL(kwk::find(view, search), expected);
  }
  {
    int search = 0;
    kumi::tuple<int> expected{0};
    TTS_EQUAL(kwk::find(view, search), expected);
  }
  {
    int search = 10;
    kumi::tuple<int> expected{5};
    TTS_EQUAL(kwk::find(view, search), expected);
  }
  {
    int search = 11;
    kumi::tuple<int> expected{-1};
    TTS_EQUAL(kwk::find(view, search), expected);
  }
  {
    int search = (input_size-1)*2;
    kumi::tuple<int> expected{input_size-1};
    TTS_EQUAL(kwk::find(view, search), expected);
  }
  {
    int search = input_size*2;
    kumi::tuple<int> expected{-1};
    TTS_EQUAL(kwk::find(view, search), expected);
  }
};

TTS_CASE("Check for kwk::find_if(In, func) 1D")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 2; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  {
    int search = -10;
    kumi::tuple<int> expected{-1};
    std::size_t count = 0;
    auto func = [&](auto item){ ++count; return (item/2 == search); };
    TTS_EQUAL(kwk::find_if(view, func), expected);
    TTS_EQUAL(count, input_size);
  }
  {
    int search = -1;
    kumi::tuple<int> expected{-1};
    std::size_t count = 0;
    auto func = [&](auto item){ ++count; return (item/2 == search); };
    TTS_EQUAL(kwk::find_if(view, func), expected);
    TTS_EQUAL(count, input_size);
  }
  {
    int search = 0;
    kumi::tuple<int> expected{0};
    std::size_t count = 0;
    auto func = [&](auto item){ ++count; return (item/2 == search); };
    TTS_EQUAL(kwk::find_if(view, func), expected);
    TTS_EQUAL(count, 1UL);
  }
  {
    int search = 1;
    kumi::tuple<int> expected{1};
    std::size_t count = 0;
    auto func = [&](auto item){ ++count; return (item/2 == search); };
    TTS_EQUAL(kwk::find_if(view, func), expected);
    TTS_EQUAL(count, 2U);
  }
  {
    int search = input_size-1;
    kumi::tuple<int> expected{input_size-1};
    std::size_t count = 0;
    auto func = [&](auto item){ ++count; return (item/2 == search); };
    TTS_EQUAL(kwk::find_if(view, func), expected);
    TTS_EQUAL(count, input_size);
  }
  {
    int search = input_size;
    kumi::tuple<int> expected{-1};
    std::size_t count = 0;
    auto func = [&](auto item){ ++count; return (item/2 == search); };
    TTS_EQUAL(kwk::find_if(view, func), expected);
    TTS_EQUAL(count, input_size);
  }
  {
    int search = 478123;
    kumi::tuple<int> expected{-1};
    std::size_t count = 0;
    auto func = [&](auto item){ ++count; return (item/2 == search); };
    TTS_EQUAL(kwk::find_if(view, func), expected);
    TTS_EQUAL(count, input_size);
  }
};

TTS_CASE("Check for kwk::find_if_not(In, func) 1D")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 3; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  {
    int search = -10;
    kumi::tuple<int> expected{0};
    std::size_t count = 0;
    // First element that has (item/3 >= search) i.e. index >= search
    auto func = [&](auto item){ ++count; return (item/3 < search); };
    TTS_EQUAL(kwk::find_if_not(view, func), expected);
    TTS_EQUAL(count, 1UL); // 0, 1, 2
  }
  {
    int search = -1;
    kumi::tuple<int> expected{0};
    std::size_t count = 0;
    // First element that has (item/3 >= search) i.e. index >= search
    auto func = [&](auto item){ ++count; return (item/3 < search); };
    TTS_EQUAL(kwk::find_if_not(view, func), expected);
    TTS_EQUAL(count, 1UL); // 0, 1, 2
  }
  {
    int search = 0;
    kumi::tuple<int> expected{0};
    std::size_t count = 0;
    // First element that has (item/3 >= search) i.e. index >= search
    auto func = [&](auto item){ ++count; return (item/3 < search); };
    TTS_EQUAL(kwk::find_if_not(view, func), expected);
    TTS_EQUAL(count, 1UL); // 0, 1, 2
  }
  {
    int search = 1;
    kumi::tuple<int> expected{1};
    std::size_t count = 0;
    // First element that has (item/3 >= search) i.e. index >= search
    auto func = [&](auto item){ ++count; return (item/3 < search); };
    TTS_EQUAL(kwk::find_if_not(view, func), expected);
    TTS_EQUAL(count, 2UL);
  }
  {
    int search = input_size-1;
    kumi::tuple<int> expected{input_size-1};
    std::size_t count = 0;
    // First element that has (item/3 >= search) i.e. index >= search
    auto func = [&](auto item){ ++count; return (item/3 < search); };
    TTS_EQUAL(kwk::find_if_not(view, func), expected);
    TTS_EQUAL(count, input_size);
  }
  {
    int search = input_size;
    kumi::tuple<int> expected{-1};
    std::size_t count = 0;
    // First element that has (item/3 >= search) i.e. index >= search
    auto func = [&](auto item){ ++count; return (item/3 < search); };
    TTS_EQUAL(kwk::find_if_not(view, func), expected);
    TTS_EQUAL(count, input_size);
  }
  {
    int search = input_size + 20000;
    kumi::tuple<int> expected{-1};
    std::size_t count = 0;
    // First element that has (item/3 >= search) i.e. index >= search
    auto func = [&](auto item){ ++count; return (item/3 < search); };
    TTS_EQUAL(kwk::find_if_not(view, func), expected);
    TTS_EQUAL(count, input_size);
  }
};

// TODO?: Implement find_last_of?
TTS_CASE("Check for kwk::find_first_of(In, In) 1D")
{
  const std::size_t input_size = 40;
  std::array<int, input_size> input;
  // for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 3; }

  for (std::size_t i = 0; i < input_size/2; ++i) { input[i] = i * 3; }
  for (std::size_t i = 0; i < input_size/2; ++i)
  {
    input[i + input_size/2] = (input_size/2 - i - 1) * 3;
  }

  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  {
    std::vector<int> search{-10};
    kumi::tuple<int> expected{-1};
    auto sv = kwk::view{kwk::source = search, kwk::of_size(search.size())};
    TTS_EQUAL(kwk::find_first_of(view, sv), expected);
  }
  {
    std::vector<int> search{-10, -5, 2, 8, 11};
    kumi::tuple<int> expected{-1};
    auto sv = kwk::view{kwk::source = search, kwk::of_size(search.size())};
    TTS_EQUAL(kwk::find_first_of(view, sv), expected);
  }
  {
    std::vector<int> search{-10, -5, 0, 8, 11};
    kumi::tuple<int> expected{0};
    auto sv = kwk::view{kwk::source = search, kwk::of_size(search.size())};
    TTS_EQUAL(kwk::find_first_of(view, sv), expected);
  }
  {
    std::vector<int> search{-10, -5, (input_size/2 - 1)*3, 8, 11};
    kumi::tuple<int> expected{input_size/2 - 1};
    auto sv = kwk::view{kwk::source = search, kwk::of_size(search.size())};
    TTS_EQUAL(kwk::find_first_of(view, sv), expected);
  }
  {
    std::vector<int> search{-10, (input_size/2)*3, -5, 8, 11};
    kumi::tuple<int> expected{-1};
    auto sv = kwk::view{kwk::source = search, kwk::of_size(search.size())};
    TTS_EQUAL(kwk::find_first_of(view, sv), expected);
  }
  {
    std::vector<int> search{-10, -5, (input_size+87845)*3, 8, 11};
    kumi::tuple<int> expected{-1};
    auto sv = kwk::view{kwk::source = search, kwk::of_size(search.size())};
    TTS_EQUAL(kwk::find_first_of(view, sv), expected);
  }
};


TTS_CASE("Check for kwk::find_last(In, value) 1D")
{
  const std::size_t input_size = 40;
  // e.g.: 0 3 6 9 12 12 9 6 3 0 
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size/2; ++i) { input[i] = i * 3; }
  for (std::size_t i = 0; i < input_size/2; ++i)
  {
    input[i + input_size/2] = (input_size/2 - i - 1) * 3;
  }

  // for (std::size_t i = 0; i < input_size; ++i)
  // {
  //   std::cout << input[i] << " ";
  // }
  // std::cout << "\n";

  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  {
    int search{-10};
    kumi::tuple<int> expected{input_size};
    TTS_EQUAL(kwk::find_last(view, search), expected);
    // TTS_EQUAL(kwk::find_last_if(view, [&](auto e){return e == search;}), expected);
  }
  {
    int search{-1};
    kumi::tuple<int> expected{input_size};
    TTS_EQUAL(kwk::find_last(view, search), expected);
  }
  {
    int search{0};
    kumi::tuple<int> expected{input_size - 1};
    TTS_EQUAL(kwk::find_last(view, search), expected);
  }
  {
    int search{1};
    kumi::tuple<int> expected{input_size};
    TTS_EQUAL(kwk::find_last(view, search), expected);
  }
  {
    int search{3};
    kumi::tuple<int> expected{input_size - 2};
    TTS_EQUAL(kwk::find_last(view, search), expected);
  }
  {
    int search{(input_size/2 - 1) * 3};
    kumi::tuple<int> expected{input_size/2};
    TTS_EQUAL(kwk::find_last(view, search), expected);
  }
  {
    int search{input_size + 1000};
    kumi::tuple<int> expected{input_size};
    TTS_EQUAL(kwk::find_last(view, search), expected);
  }
};

TTS_CASE("Check for kwk::find_last_if(In, func) 1D")
{
  const std::size_t input_size = 40;
  // e.g.: 0 3 6 9 12 12 9 6 3 0 
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size/2; ++i) { input[i] = i * 3; }
  for (std::size_t i = 0; i < input_size/2; ++i)
  {
    input[i + input_size/2] = (input_size/2 - i - 1) * 3;
  }

  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  kumi::tuple<int> expect_not_found{input_size};

  {
    int search{-10};
    auto func = [=](auto const& e){ return e == search; };
    TTS_EQUAL(kwk::find_last_if(view, func), expect_not_found);
  }
  {
    int search{-1};
    auto func = [&](auto e){ return e == search; };
    TTS_EQUAL(kwk::find_last_if(view, func), expect_not_found);
  }
  {
    int search{0};
    kumi::tuple<int> expected{input_size - 1};
    auto func = [&](auto e){ return e == search; };
    TTS_EQUAL(kwk::find_last_if(view, func), expected);
  }
  {
    int search{1};
    auto func = [&](auto e){ return e == search; };
    TTS_EQUAL(kwk::find_last_if(view, func), expect_not_found);
  }
  {
    int search{3};
    kumi::tuple<int> expected{input_size - 2};
    auto func = [&](auto e){ return e == search; };
    TTS_EQUAL(kwk::find_last_if(view, func), expected);
  }
  {
    int search{(input_size/2 - 1) * 3};
    kumi::tuple<int> expected{input_size/2};
    auto func = [&](auto e){ return e == search; };
    TTS_EQUAL(kwk::find_last_if(view, func), expected);
  }
  {
    int search{input_size + 1000};
    auto func = [&](auto e){ return e == search; };
    TTS_EQUAL(kwk::find_last_if(view, func), expect_not_found);
  }
};

TTS_CASE("Check for kwk::find_last_if_not(In, func) 1D")
{
  const std::size_t input_size = 40;
  // e.g.: 0 3 6 9 12 12 9 6 3 0 
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size/2; ++i) { input[i] = i * 3; }
  for (std::size_t i = 0; i < input_size/2; ++i)
  {
    input[i + input_size/2] = (input_size/2 - i - 1) * 3;
  }

  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  kumi::tuple<int> expect_not_found{input_size};

  {
    int search{-10};
    auto func = [=](auto const& e){ return e != search; };
    TTS_EQUAL(kwk::find_last_if_not(view, func), expect_not_found);
  }
  {
    int search{-1};
    auto func = [&](auto e){ return e != search; };
    TTS_EQUAL(kwk::find_last_if_not(view, func), expect_not_found);
  }
  {
    int search{0};
    kumi::tuple<int> expected{input_size - 1};
    auto func = [&](auto e){ return e != search; };
    TTS_EQUAL(kwk::find_last_if_not(view, func), expected);
  }
  {
    int search{1};
    auto func = [&](auto e){ return e != search; };
    TTS_EQUAL(kwk::find_last_if_not(view, func), expect_not_found);
  }
  {
    int search{3};
    kumi::tuple<int> expected{input_size - 2};
    auto func = [&](auto e){ return e != search; };
    TTS_EQUAL(kwk::find_last_if_not(view, func), expected);
  }
  {
    int search{(input_size/2 - 1) * 3};
    kumi::tuple<int> expected{input_size/2};
    auto func = [&](auto e){ return e != search; };
    TTS_EQUAL(kwk::find_last_if_not(view, func), expected);
  }
  {
    int search{input_size + 1000};
    auto func = [&](auto e){ return e != search; };
    TTS_EQUAL(kwk::find_last_if_not(view, func), expect_not_found);
  }
};
