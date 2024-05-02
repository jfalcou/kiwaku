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
#include <algorithm>
#include <array>

// Tests only valid for 1D tables/views
// TODO: at some point, make algorithms work for nD

// ========== LOWER BOUND ==========
#define LB_VALID(input, view, search, expected)                                                   \
  {                                                                                               \
    /*Compare values*/                                                                            \
    auto r = std::array<std::size_t, 1>{expected};                                                \
    TTS_ALL_EQUAL(*kwk::lower_bound(view, search), r);                                            \
    /*Compare with std implementation*/                                                           \
    TTS_EQUAL ( (*kwk::lower_bound(view, search))[0]                                              \
              , (std::lower_bound(input.begin(), input.end(), search) - input.begin())            \
              );                                                                                  \
  }

#define LB_ERROR(input, view, search)                                                             \
  {                                                                                               \
    TTS_EQUAL(kwk::lower_bound(view, search), std::nullopt);                                      \
    TTS_EQUAL(std::lower_bound(input.begin(), input.end(), search), input.end());                 \
  }

// ========== LOWER BOUND with function ==========
#define LB_VALID_FUNC(input, view, func, search, expected)                                        \
  {                                                                                               \
    /*Compare values*/                                                                            \
    std::array<std::size_t, 1> r{expected};                                                       \
    TTS_ALL_EQUAL(*kwk::lower_bound(view, search, func), r);                                      \
    /*Compare with std implementation: compare found indexes*/                                    \
    TTS_EQUAL ( (*kwk::lower_bound(view, search, func))[0] \
              , (std::lower_bound(input.begin(), input.end(), search, func) - input.begin())      \
              );                                                                                  \
  }

#define LB_ERROR_FUNC(input, view, func, search)                                                  \
  {                                                                                               \
    TTS_EQUAL(kwk::lower_bound(view, search, func), std::nullopt);                                \
    TTS_EQUAL(std::lower_bound(input.begin(), input.end(), search, func), input.end());           \
  }

// ========== UPPER BOUND ==========
#define UB_VALID(input, view, search, expected)                                                   \
  {                                                                                               \
    /*Compare values*/                                                                            \
    auto r = std::array<std::size_t, 1>{expected};                                                \
    TTS_ALL_EQUAL(*kwk::upper_bound(view, search), r);                                            \
    /*Compare with std implementation*/                                                           \
    TTS_EQUAL ( (*kwk::upper_bound(view, search))[0]                                              \
              , (std::upper_bound(input.begin(), input.end(), search) - input.begin())            \
              );                                                                                  \
  }

#define UB_ERROR(input, view, search)                                                             \
  {                                                                                               \
    TTS_EQUAL(kwk::upper_bound(view, search), std::nullopt);                                      \
    TTS_EQUAL(std::upper_bound(input.begin(), input.end(), search), input.end());                 \
  }

// ========== UPPER BOUND with function ==========
#define UB_VALID_FUNC(input, view, func, search, expected)                                        \
  {                                                                                               \
    /*Compare values*/                                                                            \
    std::array<std::size_t, 1> r{expected};                                                       \
    TTS_ALL_EQUAL(*kwk::upper_bound(view, search, func), r);                                      \
    /*Compare with std implementation: compare found indexes*/                                    \
    TTS_EQUAL ( (*kwk::upper_bound(view, search, func))[0] \
              , (std::upper_bound(input.begin(), input.end(), search, func) - input.begin())      \
              );                                                                                  \
  }

#define UB_ERROR_FUNC(input, view, func, search)                                                  \
  {                                                                                               \
    TTS_EQUAL(kwk::upper_bound(view, search, func), std::nullopt);                                \
    TTS_EQUAL(std::upper_bound(input.begin(), input.end(), search, func), input.end());           \
  }


// ========== BINARY SEARCH ==========
#define BS(input, view, search, expected)                                                         \
  {                                                                                               \
    bool res = kwk::binary_search(view, search);                                                  \
    TTS_EQUAL(res, expected);                                                                     \
    bool res_std = std::binary_search(input.begin(), input.end(), search);                        \
    TTS_EQUAL(res, res_std);                                                                      \
  }

#define BS_FCT(input, view, func, search, expected)                                               \
  {                                                                                               \
    bool res = kwk::binary_search(view, search, func);                                            \
    TTS_EQUAL(res, expected);                                                                     \
    bool res_std = std::binary_search(input.begin(), input.end(), search, func);                  \
    TTS_EQUAL(res, res_std);                                                                      \
  }



TTS_CASE("Check for kwk::lower_bound(In, value) 1D ---------")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  // lower_bound returns the index of the first element above the specified value
  LB_VALID(input, view, -2984612, 0);
  LB_VALID(input, view, -2, 0);
  LB_VALID(input, view, 0, 0);
  LB_VALID(input, view, 5, 5);
  LB_VALID(input, view, 19, 19);
  LB_ERROR(input, view, 20);
  LB_ERROR(input, view, 21);
  LB_ERROR(input, view, 78456465);
};

TTS_CASE("Check for kwk::lower_bound(In, value, func) 1D with function")
{
  {
    const std::size_t input_size = 20;
    std::array<int, input_size> input;
    for (std::size_t i = 0; i < input_size; ++i) { input[i] = (i - 10) * 2; }
    auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};
    auto func = [](auto const& input_, auto const& element_) { return input_ < (element_ - 10) * 2; };

    LB_VALID_FUNC(input, view, func, -2984612, 0);
    LB_VALID_FUNC(input, view, func, -2, 0);
    LB_VALID_FUNC(input, view, func, 0, 0);
    LB_VALID_FUNC(input, view, func, 5, 5);
    LB_VALID_FUNC(input, view, func, 9, 9);
    LB_VALID_FUNC(input, view, func, 19, 19);
    LB_ERROR_FUNC(input, view, func, 20);
    LB_ERROR_FUNC(input, view, func, 21);
    LB_ERROR_FUNC(input, view, func, 78456465);
  }
  // std::cout << (*kwk::lower_bound(view, 21))[0] << std::endl;
};

TTS_CASE("Check for kwk::lower_bound 1D: size 0")
{
  const std::size_t input_size = 0;
  std::array<int, input_size> input;
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};
  // auto func = [](auto const& input_, auto const& element_) { return input_ < (element_ - 10) * 2; };

  LB_ERROR(input, view, -2984612);
  // LB_ERROR(input, view, -2);
  // LB_ERROR(input, view, 0);
  // LB_ERROR(input, view, 5);
  // LB_ERROR(input, view, 19);
  // LB_ERROR(input, view, 20);
  // LB_ERROR(input, view, 21);
  // LB_ERROR(input, view, 78456465);

  // LB_ERROR_FUNC(input, view, func, -2984612);
  // LB_ERROR_FUNC(input, view, func, -2);
  // LB_ERROR_FUNC(input, view, func, 0);
  // LB_ERROR_FUNC(input, view, func, 5);
  // LB_ERROR_FUNC(input, view, func, 9);
  // LB_ERROR_FUNC(input, view, func, 19);
  // LB_ERROR_FUNC(input, view, func, 20);
  // LB_ERROR_FUNC(input, view, func, 21);
  // LB_ERROR_FUNC(input, view, func, 78456465);
};


TTS_CASE("Check for kwk::upper_bound(In, value) 1D ---------")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  // upper_bound returns the index of the first element above the specified value
  UB_VALID(input, view, -895, 0);
  UB_VALID(input, view, -78495, 0);
  UB_VALID(input, view, -1, 0);
  UB_VALID(input, view, 0, 1);
  UB_VALID(input, view, 5, 6);
  UB_VALID(input, view, 18, 19);
  UB_ERROR(input, view, 19);
  UB_ERROR(input, view, 21);
  UB_ERROR(input, view, 2000);
  UB_ERROR(input, view, 78456465);
};


TTS_CASE("Check for kwk::upper_bound(In, value) 1D with function")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 3 - 20; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};
  auto func = [](auto const& search_, auto const& element_) { return search_ * 3 - 20 < element_; };

  UB_VALID_FUNC(input, view, func, -895, 0);
  UB_VALID_FUNC(input, view, func, -78495, 0);
  UB_VALID_FUNC(input, view, func, -1, 0);
  UB_VALID_FUNC(input, view, func, 0, 1);
  UB_VALID_FUNC(input, view, func, 5, 6);
  UB_VALID_FUNC(input, view, func, 18, 19);
  UB_ERROR_FUNC(input, view, func, 19);
  UB_ERROR_FUNC(input, view, func, 2000);
  UB_ERROR_FUNC(input, view, func, 78456465);
};

// TTS_CASE("Check for kwk::upper_bound 1D: size = 0")
// {
//   const std::size_t input_size = 0;
//   std::array<int, input_size> input;
//   for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 3 - 20; }
//   auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};
//   auto func = [](auto const& search_, auto const& element_) { return search_ * 3 - 20 < element_; };

//   UB_ERROR_FUNC(input, view, func, -895);
//   UB_ERROR_FUNC(input, view, func, -78495);
//   UB_ERROR_FUNC(input, view, func, -1);
//   UB_ERROR_FUNC(input, view, func, 0);
//   UB_ERROR_FUNC(input, view, func, 5);
//   UB_ERROR_FUNC(input, view, func, 18);
//   UB_ERROR_FUNC(input, view, func, 19);
//   UB_ERROR_FUNC(input, view, func, 2000);
//   UB_ERROR_FUNC(input, view, func, 78456465);

//   UB_ERROR(input, view, -895);
//   UB_ERROR(input, view, -78495);
//   UB_ERROR(input, view, -1);
//   UB_ERROR(input, view, 0);
//   UB_ERROR(input, view, 5);
//   UB_ERROR(input, view, 18);
//   UB_ERROR(input, view, 19);
//   UB_ERROR(input, view, 21);
//   UB_ERROR(input, view, 2000);
//   UB_ERROR(input, view, 78456465);
// };

TTS_CASE("Check for kwk::binary_search(In, value) 1D ---------")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 3; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  // Ensures that the return value is expected
  BS(input, view, -895     , false);
  BS(input, view, -78495   , false);
  BS(input, view, -1       , false);
  BS(input, view, 0        , true);
  BS(input, view, 5*3      , true);
  BS(input, view, 18*3-1   , false);
  BS(input, view, 18*3     , true);
  BS(input, view, 18*3+1   , false);
  BS(input, view, 19*3     , true);
  BS(input, view, 20*3     , false);
  BS(input, view, 2000     , false);
  BS(input, view, 78456465 , false);
};

TTS_CASE("Check for kwk::binary_search(In, value) 1D with function v1")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 2; } //  * 3 - 20
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  auto func = [](auto const& search_, auto const& element_) { return search_ < element_; };

  BS_FCT(input, view, func, -895, false);
  BS_FCT(input, view, func, -78495, false);
  BS_FCT(input, view, func, -1, false);
  BS_FCT(input, view, func, 0, true);
  BS_FCT(input, view, func, 5, false);
  BS_FCT(input, view, func, 18, true);
  BS_FCT(input, view, func, 19, false);
  BS_FCT(input, view, func, 20, true);
  BS_FCT(input, view, func, 21, false)
  BS_FCT(input, view, func, 22, true)
  BS_FCT(input, view, func, 23, false)
  BS_FCT(input, view, func, 24, true)
  BS_FCT(input, view, func, 29, false)
  BS_FCT(input, view, func, 36, true)
  BS_FCT(input, view, func, 2000, false);
  BS_FCT(input, view, func, 78456465, false);
  BS_FCT(input, view, func, -895, false);
};
