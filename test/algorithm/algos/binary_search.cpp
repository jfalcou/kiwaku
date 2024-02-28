//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <cstdlib>
#include <kwk/algorithm/algos/binary_search.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

// Tests only valid for 1D tables/views

namespace
{
  struct check_lower_bound
  {
    /// @brief Checks that "search" is found at the position "expected". 
    /// @param view kiwaku view
    /// @param search value to search in the view
    /// @param expected expected returned position
    static void valid(auto& view, int search, std::size_t expected)
    {
      // std::optional<std::array<int, 1>> result;
      auto res = kwk::lower_bound(view, search);
      std::array<std::size_t, 1> exp{expected};
      TTS_ALL_EQUAL(*res, exp);
    }

    static void valid(auto& view, int search, std::size_t expected, auto& func)
    {
      // std::optional<std::array<int, 1>> result;
      auto res = kwk::lower_bound(view, search, func);
      std::array<std::size_t, 1> exp{expected};
      TTS_ALL_EQUAL(*res, exp);
    }

    /// @brief Checks that "search" is not found in the kiwaku view. 
    /// @param view kiwaku view
    /// @param search value to search in the view
    static void invalid(auto& view, int search)
    {
      auto res = kwk::lower_bound(view, search);
      TTS_EXPECT(res == std::nullopt);
    }

    static void invalid(auto& view, int search, auto& func)
    {
      auto res = kwk::lower_bound(view, search, func);
      TTS_EXPECT(res == std::nullopt);
    }
  };
}

TTS_CASE("Check for kwk::lower_bound(In, value) 1D")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  check_lower_bound::valid(view, -2984612, 0);
  check_lower_bound::valid(view, -2, 0);
  check_lower_bound::valid(view, 5, 5);
  check_lower_bound::valid(view, 0, 0);
  check_lower_bound::valid(view, 19, 19);
  check_lower_bound::invalid(view, 20);
  check_lower_bound::invalid(view, 21);
  check_lower_bound::invalid(view, 78456465);
};


TTS_CASE("Check for kwk::lower_bound(In, value, func) 1D original")
{
  int data[2];
  int vdata[] = {0};

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  auto res = kwk::lower_bound(d, 1, [](auto e, auto s)
  {
    return e < (s-1);
  });

  if(res)
    TTS_ALL_EQUAL(*res, vdata);
  else
    TTS_EXPECT(res == std::nullopt);
};


TTS_CASE("Check for kwk::lower_bound(In, value, func) 1D bis")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = (i - 10) * 2; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  auto func = [](auto e, auto val) { return e < (val - 10) * 2; };

  check_lower_bound::valid(view, -2984612, 0, func);
  check_lower_bound::valid(view, -2, 0, func);
  check_lower_bound::valid(view, 5, 5, func);
  check_lower_bound::valid(view, 0, 0, func);
  check_lower_bound::valid(view, 9, 9, func);
  check_lower_bound::valid(view, 19, 19, func);
  check_lower_bound::invalid(view, 20, func);
  check_lower_bound::invalid(view, 21, func);
  check_lower_bound::invalid(view, 78456465, func);
};



TTS_CASE("Check for kwk::upper_bound(In, value) 1D")
{
  int data[2];
  int vdata[] = {1};

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  auto res = kwk::upper_bound(d, 0);

  if(res)
    TTS_ALL_EQUAL(*res, vdata);
  else
    TTS_EXPECT(res == std::nullopt);
};



TTS_CASE("Check for kwk::upper_bound(In, value, func) 1D")
{
  int data[2];
  int vdata[] = {1};

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  auto res = kwk::upper_bound(d, 0, [](auto e, auto s)
  {
    return e > (s+1);
  });

  if(res)
    TTS_ALL_EQUAL(*res, vdata);
  else
    TTS_EXPECT(res == std::nullopt);
};


TTS_CASE("Check for kwk::binary_search(In, value) 1D")
{
  int data[2];
  int needles[] = {0,1,2};

  std::array<bool,3> res;
  std::array<bool,3> vres {true, true, false};

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  size_t i = 0;
  for (auto needle : needles)
    if(kwk::binary_search(d, needle))
      res[i++] = true;
    else
      res[i++] = false;

  TTS_ALL_EQUAL(res, vres);
};


TTS_CASE("Check for kwk::binary_search(In, value, func) 1D")
{
  int data[2];
  int needles[] = {0,1,2};

  std::array<bool,3> res;
  std::array<bool,3> vres {true, true, false};

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};

  size_t i = 0;
  for (auto needle : needles)
    if(kwk::binary_search(d, needle, [](auto e, auto s){return e < s;}))
      res[i++] = true;
    else
      res[i++] = false;

  TTS_ALL_EQUAL(res, vres);
};
