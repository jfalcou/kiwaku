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

namespace
{
  enum algo_type {lower_bound, upper_bound};

  struct check_bound
  {
    /// @brief Checks that "search" is found at the position "expected". 
    /// @param view kiwaku view
    /// @param search value to search in the view
    /// @param expected expected returned position
    static void valid(algo_type type, auto& view, int search, std::size_t expected)
    {
      std::optional<std::array<int, 1>> result;
      if (type == algo_type::lower_bound) result = kwk::lower_bound(view, search);
      if (type == algo_type::upper_bound) result = kwk::upper_bound(view, search);
      std::array<std::size_t, 1> exp{expected};
      TTS_ALL_EQUAL(*result, exp);
    }

    static void valid(algo_type type,auto& view, int search, std::size_t expected, auto& func)
    {
      std::optional<std::array<int, 1>> result;
      if (type == algo_type::lower_bound) result = kwk::lower_bound(view, search, func);
      if (type == algo_type::upper_bound) result = kwk::upper_bound(view, search, func);
      std::array<std::size_t, 1> exp{expected};
      TTS_ALL_EQUAL(*result, exp);
    }

    /// @brief Checks that "search" is not found in the kiwaku view. 
    /// @param view kiwaku view
    /// @param search value to search in the view
    static void invalid(algo_type type,auto& view, int search)
    {
      std::optional<std::array<int, 1>> result;
      if (type == algo_type::lower_bound) result = kwk::lower_bound(view, search);
      if (type == algo_type::upper_bound) result = kwk::upper_bound(view, search);
      TTS_EXPECT(result == std::nullopt);
    }

    static void invalid(algo_type type,auto& view, int search, auto& func)
    {
      std::optional<std::array<int, 1>> result;
      if (type == algo_type::lower_bound) result = kwk::lower_bound(view, search, func);
      if (type == algo_type::upper_bound) result = kwk::upper_bound(view, search, func);
      TTS_EXPECT(result == std::nullopt);
    }
  };

// ========== LOWER BOUND ==========
#define LB_VALID(view, search, expected)                                                          \
  {                                                                                               \
    auto r = std::array<std::size_t, 1>{expected};                                                \
    TTS_ALL_EQUAL(*kwk::lower_bound(view, search), r);                                            \
  }

#define LB_VALID_std(input, view, search)                                                         \
  {                                                                                               \
    TTS_EQUAL ( (*kwk::lower_bound(view, search))[0]                                              \
              , (std::lower_bound(input.begin(), input.end(), search) - input.begin())            \
              );                                                                                  \
  }

#define LB_ERROR(view, search)                                                                    \
  {                                                                                               \
    TTS_EQUAL(kwk::lower_bound(view, search), std::nullopt);                                      \
  }

#define LB_ERROR_std(input, view, search)                                                         \
  {                                                                                               \
    TTS_EQUAL(kwk::lower_bound(view, search), std::nullopt);                                      \
    TTS_EQUAL(std::lower_bound(input.begin(), input.end(), search), input.end());                 \
  }


// ========== LOWER BOUND with function ==========
#define LB_VALID_FUNC(view, func, search, expected)                                               \
  {                                                                                               \
    auto r = std::array<std::size_t, 1>{expected};                                                \
    TTS_ALL_EQUAL(*kwk::lower_bound(view, search, func), r);                                      \
  }

#define LB_VALID_FUNC_std(input, view, func, search)                                              \
  {                                                                                               \
    /*Compare found indexes*/                                                                     \
    TTS_EQUAL ( (*kwk::lower_bound(view, search, func))[0] \
              , (std::lower_bound(input.begin(), input.end(), search, func) - input.begin())      \
              );                                                                                  \
  }

#define LB_ERROR_FUNC(view, func, search)                                                         \
  {                                                                                               \
    TTS_EQUAL(kwk::lower_bound(view, search, func), std::nullopt);                                \
  }

#define LB_ERROR_FUNC_std(input, view, func, search)                                              \
  {                                                                                               \
    TTS_EQUAL(kwk::lower_bound(view, search, func), std::nullopt);                                \
    TTS_EQUAL(std::lower_bound(input.begin(), input.end(), search, func), input.end());           \
  }


// ========== UPPER BOUND ==========
#define UB_VALID(view, search, expected)                                                          \
  {                                                                                               \
    auto r = std::array<std::size_t, 1>{expected};                                                \
    TTS_ALL_EQUAL(*kwk::upper_bound(view, search), r);                                            \
  }

#define UB_VALID_std(input, view, search)                                                         \
  {                                                                                               \
    TTS_EQUAL ( (*kwk::upper_bound(view, search))[0]                                              \
              , (std::upper_bound(input.begin(), input.end(), search) - input.begin())            \
              );                                                                                  \
  }

#define UB_ERROR(view, search)                                                                    \
  {                                                                                               \
    TTS_EQUAL(kwk::upper_bound(view, search), std::nullopt);                                      \
  }

#define UB_ERROR_std(input, view, search)                                                         \
  {                                                                                               \
    TTS_EQUAL(kwk::upper_bound(view, search), std::nullopt);                                      \
    TTS_EQUAL(std::upper_bound(input.begin(), input.end(), search), input.end());                 \
  }


// ========== UPPER BOUND with function ==========
#define UB_VALID_FUNC(view, func, search, expected)                                               \
  {                                                                                               \
    auto r = std::array<std::size_t, 1>{expected};                                                \
    TTS_ALL_EQUAL(*kwk::upper_bound(view, search, func), r);                                      \
  }

#define UB_VALID_FUNC_std(input, view, func, search)                                              \
  {                                                                                               \
    /*Compare found indexes*/                                                                     \
    TTS_EQUAL ( (*kwk::upper_bound(view, search, func))[0] \
              , (std::upper_bound(input.begin(), input.end(), search, func) - input.begin())      \
              );                                                                                  \
  }

#define UB_ERROR_FUNC(view, func, search)                                                         \
  {                                                                                               \
    TTS_EQUAL(kwk::upper_bound(view, search, func), std::nullopt);                                \
  }

#define UB_ERROR_FUNC_std(input, view, func, search)                                              \
  {                                                                                               \
    TTS_EQUAL(kwk::upper_bound(view, search, func), std::nullopt);                                \
    TTS_EQUAL(std::upper_bound(input.begin(), input.end(), search, func), input.end());           \
  }





// ========== BINARY SEARCH ==========
#define BS(view, search, expected)                                                                \
  {                                                                                               \
    bool result = kwk::binary_search(view, search);                                               \
    TTS_EQUAL(result, expected);                                                                  \
  }

#define BS_FCT(view, search, expected, func)                                                      \
  {                                                                                               \
    bool result = kwk::binary_search(view, search, func);                                         \
    TTS_EQUAL(result, expected);                                                                  \
  }

#define BS_std(input, view, search)                                                               \
  {                                                                                               \
    bool res = kwk::binary_search(view, search);                                                  \
    bool res_std = std::binary_search(input.begin(), input.end(), search);                        \
    TTS_EQUAL(res, res_std);                                                                      \
  }

#define BS_FCT_std(input, view, search, func)                                                     \
  {                                                                                               \
    bool res = kwk::binary_search(view, search, func);                                            \
    bool res_std = std::binary_search(input.begin(), input.end(), search, func);                  \
    TTS_EQUAL(res, res_std);                                                                      \
  }
}

TTS_CASE("Check for kwk::lower_bound(In, value) 1D ---------")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};


  // std::optional<std::array<int, 1>> result;
  // if (type == algo_type::lower_bound) result = kwk::lower_bound(view, search);
  // if (type == algo_type::upper_bound) result = kwk::upper_bound(view, search);
  // std::array<std::size_t, 1> exp{expected};
  // TTS_ALL_EQUAL(*result, exp);

  auto eres = std::array<std::size_t, 1>{0};
  TTS_ALL_EQUAL(*(kwk::lower_bound(view, -2984612)), eres);

  // lower_bound returns the index of the first element above the specified value
  LB_VALID(view, -2984612, 0);
  LB_VALID(view, -2, 0);
  LB_VALID(view, 0, 1);
  LB_VALID(view, 5, 5);
  LB_VALID(view, 19, 19);
  LB_ERROR(view, 20);
  LB_ERROR(view, 21);
  LB_ERROR(view, 78456465);

  LB_VALID_std(input, view, -2984612);
  LB_VALID_std(input, view, -2);
  LB_VALID_std(input, view, 0);
  LB_VALID_std(input, view, 5);
  LB_VALID_std(input, view, 19);
  LB_ERROR_std(input, view, 20);
  LB_ERROR_std(input, view, 21);
  LB_ERROR_std(input, view, 78456465);
};


TTS_CASE("Check for kwk::lower_bound(In, value, func) 1D with function")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = (i - 10) * 2; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  auto func = [](auto const& input_, auto const& element_) { return input_ < (element_ - 10) * 2; };

  LB_VALID_FUNC(view, func, -2984612, 0);
  LB_VALID_FUNC(view, func, -2, 0);
  LB_VALID_FUNC(view, func, 0, 0);
  LB_VALID_FUNC(view, func, 5, 5);
  LB_VALID_FUNC(view, func, 9, 9);
  LB_VALID_FUNC(view, func, 19, 19);
  LB_ERROR_FUNC(view, func, 20);
  LB_ERROR_FUNC(view, func, 21);
  LB_ERROR_FUNC(view, func, 78456465);

  LB_VALID_FUNC_std(input, view, func, -2984612);
  LB_VALID_FUNC_std(input, view, func, -2);
  LB_VALID_FUNC_std(input, view, func, 0);
  LB_VALID_FUNC_std(input, view, func, 5);
  LB_VALID_FUNC_std(input, view, func, 9);
  LB_VALID_FUNC_std(input, view, func, 19);
  LB_ERROR_FUNC_std(input, view, func, 20);
  LB_ERROR_FUNC_std(input, view, func, 21);
  LB_ERROR_FUNC_std(input, view, func, 78456465);


  // std::cout << (*kwk::lower_bound(view, 21))[0] << std::endl;
};

TTS_CASE("Check for kwk::upper_bound(In, value) 1D ---------")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  // upper_bound returns the index of the first element above the specified value
  UB_VALID(view, -895, 0);
  UB_VALID(view, -78495, 0);
  UB_VALID(view, -1, 0);
  UB_VALID(view, 0, 1);
  UB_VALID(view, 5, 6);
  UB_VALID(view, 18, 19);
  UB_ERROR(view, 19);
  UB_ERROR(view, 21);
  UB_ERROR(view, 2000);
  UB_ERROR(view, 78456465);

  UB_VALID_std(input, view, -895);
  UB_VALID_std(input, view, -78495);
  UB_VALID_std(input, view, -1);
  UB_VALID_std(input, view, 0);
  UB_VALID_std(input, view, 5);
  UB_VALID_std(input, view, 18);
  UB_ERROR_std(input, view, 19);
  UB_ERROR_std(input, view, 21);
  UB_ERROR_std(input, view, 2000);
  UB_ERROR_std(input, view, 78456465);
};


TTS_CASE("Check for kwk::upper_bound(In, value) 1D with function")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 3 - 20; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  auto func = [](auto const& search_, auto const& element_) { return search_ * 3 - 20 < element_; };

  check_bound::valid  (algo_type::upper_bound, view, -895, 0, func);
  check_bound::valid  (algo_type::upper_bound, view, -78495, 0, func);
  check_bound::valid  (algo_type::upper_bound, view, -1, 0, func);
  check_bound::valid  (algo_type::upper_bound, view, 0, 1, func);
  check_bound::valid  (algo_type::upper_bound, view, 5, 6, func);
  check_bound::valid  (algo_type::upper_bound, view, 18, 19, func);
  check_bound::invalid(algo_type::upper_bound, view, 19, func);
  check_bound::invalid(algo_type::upper_bound, view, 2000, func);
  check_bound::invalid(algo_type::upper_bound, view, 78456465, func);
  // std::cout << (*kwk::upper_bound(view, 10, func))[0] << std::endl;
};

TTS_CASE("Check for kwk::binary_search(In, value) 1D ---------")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 3; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  // Ensures that the return value is expected
  BS(view, -895     , false);
  BS(view, -78495   , false);
  BS(view, -1       , false);
  BS(view, 0        , true);
  BS(view, 5*3      , true);
  BS(view, 18*3-1   , false);
  BS(view, 18*3     , true);
  BS(view, 18*3+1   , false);
  BS(view, 19*3     , true);
  BS(view, 20*3     , false);
  BS(view, 2000     , false);
  BS(view, 78456465 , false);

  BS_std(input, view, -895    );
  BS_std(input, view, -78495  );
  BS_std(input, view, -1      );
  BS_std(input, view, 0       );
  BS_std(input, view, 5*3     );
  BS_std(input, view, 18*3-1  );
  BS_std(input, view, 18*3    );
  BS_std(input, view, 18*3+1  );
  BS_std(input, view, 19*3    );
  BS_std(input, view, 20*3    );
  BS_std(input, view, 2000    );
  BS_std(input, view, 78456465);
};

TTS_CASE("Check for kwk::binary_search(In, value) 1D with function v1")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 2; } //  * 3 - 20
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  auto func = [](auto const& search_, auto const& element_) { return search_ < element_; };

  BS_FCT(view, -895, false, func);
  BS_FCT(view, -78495, false, func);
  BS_FCT(view, -1, false, func);
  BS_FCT(view, 0, true, func);
  BS_FCT(view, 5, false, func);
  BS_FCT(view, 18, true, func);
  BS_FCT(view, 19, false, func);
  BS_FCT(view, 20, true, func);
  BS_FCT(view, 21, false, func)
  BS_FCT(view, 22, true, func)
  BS_FCT(view, 23, false, func)
  BS_FCT(view, 24, true, func)
  BS_FCT(view, 29, false, func)
  BS_FCT(view, 36, true, func)
  BS_FCT(view, 2000, false, func);
  BS_FCT(view, 78456465, false, func);
  BS_FCT(view, -895, false, func);

  BS_FCT_std(input, view, -895, func);
  BS_FCT_std(input, view, -78495, func);
  BS_FCT_std(input, view, -1, func);
  BS_FCT_std(input, view, 0, func);
  BS_FCT_std(input, view, 5, func);
  BS_FCT_std(input, view, 18, func);
  BS_FCT_std(input, view, 19, func);
  BS_FCT_std(input, view, 20, func);
  BS_FCT_std(input, view, 21, func)
  BS_FCT_std(input, view, 22, func)
  BS_FCT_std(input, view, 23, func)
  BS_FCT_std(input, view, 24, func)
  BS_FCT_std(input, view, 29, func)
  BS_FCT_std(input, view, 36, func)
  BS_FCT_std(input, view, 2000, func);
  BS_FCT_std(input, view, 78456465, func);
  BS_FCT_std(input, view, -895, func);
};
