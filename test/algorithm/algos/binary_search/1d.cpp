//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#include <cstdlib>
#include <kwk/algorithm/algos/binary_search.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <algorithm>
#include <array>

// binary_search

// Only tests 1D tables/views
// TODO: at some point, make algorithms work for nD

TTS_CASE("Check for kwk::lower_bound(In, value) 1D")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  // Valid values
  {
    int search = -2984612;
    std::size_t expected = 0;
    TTS_ALL_EQUAL(*kwk::lower_bound(view, search), (std::array<std::size_t, 1>{expected}));
    TTS_EQUAL ((*kwk::lower_bound(view, search))[0]
              , std::distance(std::lower_bound(input.begin(), input.end(), search), input.begin())
              );
  }
  {
    int search = 0;
    std::size_t expected = 0;
    TTS_ALL_EQUAL(*kwk::lower_bound(view, search), (std::array<std::size_t, 1>{expected}));
    TTS_EQUAL ((*kwk::lower_bound(view, search))[0]
              , std::distance(std::lower_bound(input.begin(), input.end(), search), input.begin())
              );
  }
  {
    int search = -1;
    std::size_t expected = 0;
    TTS_ALL_EQUAL(*kwk::lower_bound(view, search), (std::array<std::size_t, 1>{expected}));
    TTS_EQUAL ((*kwk::lower_bound(view, search))[0]
              , std::distance(std::lower_bound(input.begin(), input.end(), search), input.begin())
              );
  }
  {
    int search = 10;
    std::size_t expected = 10;
    TTS_ALL_EQUAL(*kwk::lower_bound(view, search), (std::array<std::size_t, 1>{expected}));
    TTS_EQUAL ((*kwk::lower_bound(view, search))[0]
              , std::distance(input.begin(), std::lower_bound(input.begin(), input.end(), search))
              );
  }
  {
    int search = 19;
    std::size_t expected = 19;
    TTS_ALL_EQUAL(*kwk::lower_bound(view, search), (std::array<std::size_t, 1>{expected}));
    TTS_EQUAL ((*kwk::lower_bound(view, search))[0]
              , std::distance(input.begin(), std::lower_bound(input.begin(), input.end(), search))
              );
  }
  // Invalid values
  {
    int search = 20;
    TTS_EQUAL(kwk::lower_bound(view, search), std::nullopt);
    TTS_EQUAL(std::lower_bound(input.begin(), input.end(), search), input.end());
  }
  {
    int search = 78456465;
    TTS_EQUAL(kwk::lower_bound(view, search), std::nullopt);
    TTS_EQUAL(std::lower_bound(input.begin(), input.end(), search), input.end());
  }
};

TTS_CASE("Check for kwk::lower_bound(In, value, func) 1D with function")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = (i - 10) * 2; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};
  auto func = [](auto const& input_, auto const& element_) { return input_ < (element_ - 10) * 2; };

  // Valid values
  {
    /*Compare values*/
    int search = -2984612;
    std::size_t expected = 0;
    std::array<std::size_t, 1> r{expected};
    TTS_ALL_EQUAL(*kwk::lower_bound(view, search, func), r);
    /*Compare with std implementation: compare found indexes*/
    TTS_EQUAL ( (*kwk::lower_bound(view, search, func))[0]
              , (std::lower_bound(input.begin(), input.end(), search, func) - input.begin())
              );
  }
  {
    /*Compare values*/
    int search = -1;
    std::size_t expected = 0;
    std::array<std::size_t, 1> r{expected};
    TTS_ALL_EQUAL(*kwk::lower_bound(view, search, func), r);
    /*Compare with std implementation: compare found indexes*/
    TTS_EQUAL ( (*kwk::lower_bound(view, search, func))[0]
              , (std::lower_bound(input.begin(), input.end(), search, func) - input.begin())
              );
  }
  {
    /*Compare values*/
    int search = 0;
    std::size_t expected = 0;
    std::array<std::size_t, 1> r{expected};
    TTS_ALL_EQUAL(*kwk::lower_bound(view, search, func), r);
    /*Compare with std implementation: compare found indexes*/
    TTS_EQUAL ( (*kwk::lower_bound(view, search, func))[0]
              , (std::lower_bound(input.begin(), input.end(), search, func) - input.begin())
              );
  }
  {
    /*Compare values*/
    int search = 9;
    std::size_t expected = 9;
    std::array<std::size_t, 1> r{expected};
    TTS_ALL_EQUAL(*kwk::lower_bound(view, search, func), r);
    /*Compare with std implementation: compare found indexes*/
    TTS_EQUAL ( (*kwk::lower_bound(view, search, func))[0]
              , (std::lower_bound(input.begin(), input.end(), search, func) - input.begin())
              );
  }
  {
    /*Compare values*/
    int search = 19;
    std::size_t expected = 19;
    std::array<std::size_t, 1> r{expected};
    TTS_ALL_EQUAL(*kwk::lower_bound(view, search, func), r);
    /*Compare with std implementation: compare found indexes*/
    TTS_EQUAL ( (*kwk::lower_bound(view, search, func))[0]
              , (std::lower_bound(input.begin(), input.end(), search, func) - input.begin())
              );
  }

  // Erroneous values
  {
    int search = 20;
    TTS_EQUAL(kwk::lower_bound(view, search, func), std::nullopt);
    TTS_EQUAL(std::lower_bound(input.begin(), input.end(), search, func), input.end());
  }
  {
    int search = 78456465;
    TTS_EQUAL(kwk::lower_bound(view, search, func), std::nullopt);
    TTS_EQUAL(std::lower_bound(input.begin(), input.end(), search, func), input.end());
  }
};

TTS_CASE("Check for kwk::lower_bound 1D: size 0")
{
  std::vector<int> input; // will not work for an std::array of size 0
  const std::size_t input_size = 0;
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};
  auto func = [](auto const& input_, auto const& element_) { return input_ < (element_ - 10) * 2; };

  // Every value should lead to an invalid result
  {
    int search = -2984612;
    // Checks that kwk::lower_bound returns an erroneous value
    TTS_EQUAL(kwk::lower_bound(view, search), std::nullopt);
    // Checks whether we shound really expect an erroneous value (std:: being our source of truth)
    TTS_EQUAL(std::lower_bound(input.begin(), input.end(), search), input.end());
  }
  {
    int search = -1;
    TTS_EQUAL(kwk::lower_bound(view, search), std::nullopt);
    TTS_EQUAL(std::lower_bound(input.begin(), input.end(), search), input.end());
  }
  {
    int search = 0;
    TTS_EQUAL(kwk::lower_bound(view, search), std::nullopt);
    TTS_EQUAL(std::lower_bound(input.begin(), input.end(), search), input.end());
  }
  {
    int search = 19;
    TTS_EQUAL(kwk::lower_bound(view, search), std::nullopt);
    TTS_EQUAL(std::lower_bound(input.begin(), input.end(), search), input.end());
  }
  {
    int search = 20;
    TTS_EQUAL(kwk::lower_bound(view, search), std::nullopt);
    TTS_EQUAL(std::lower_bound(input.begin(), input.end(), search), input.end());
  }
  {
    int search = 78456465;
    TTS_EQUAL(kwk::lower_bound(view, search), std::nullopt);
    TTS_EQUAL(std::lower_bound(input.begin(), input.end(), search), input.end());
  }

  // With function
  {
    int search = -2984612;
    TTS_EQUAL(kwk::lower_bound(view, search, func), std::nullopt);
    TTS_EQUAL(std::lower_bound(input.begin(), input.end(), search, func), input.end());
  }
  {
    int search = -1;
    TTS_EQUAL(kwk::lower_bound(view, search, func), std::nullopt);
    TTS_EQUAL(std::lower_bound(input.begin(), input.end(), search, func), input.end());
  }
  {
    int search = 0;
    TTS_EQUAL(kwk::lower_bound(view, search, func), std::nullopt);
    TTS_EQUAL(std::lower_bound(input.begin(), input.end(), search, func), input.end());
  }
  {
    int search = 19;
    TTS_EQUAL(kwk::lower_bound(view, search, func), std::nullopt);
    TTS_EQUAL(std::lower_bound(input.begin(), input.end(), search, func), input.end());
  }
  {
    int search = 20;
    TTS_EQUAL(kwk::lower_bound(view, search, func), std::nullopt);
    TTS_EQUAL(std::lower_bound(input.begin(), input.end(), search, func), input.end());
  }
  {
    int search = 78456465;
    TTS_EQUAL(kwk::lower_bound(view, search, func), std::nullopt);
    TTS_EQUAL(std::lower_bound(input.begin(), input.end(), search, func), input.end());
  }
};


TTS_CASE("Check for kwk::upper_bound(In, value) 1D")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  // upper_bound returns the index of the first element above the specified value
  {
    int search = -78495;
    std::size_t expected = 0; 
    /*Compare values*/
    std::array<std::size_t, 1> r{expected};
    TTS_ALL_EQUAL(*kwk::upper_bound(view, search), r);
    /*Compare with std implementation*/
    TTS_EQUAL ( (*kwk::upper_bound(view, search))[0]
              , (std::upper_bound(input.begin(), input.end(), search) - input.begin())
              );     
  }
  {
    int search = -1;
    std::size_t expected = 0; 
    /*Compare values*/
    std::array<std::size_t, 1> r{expected};
    TTS_ALL_EQUAL(*kwk::upper_bound(view, search), r);
    /*Compare with std implementation*/
    TTS_EQUAL ( (*kwk::upper_bound(view, search))[0]
              , (std::upper_bound(input.begin(), input.end(), search) - input.begin())
              );     
  }
  {
    int search = 0;
    std::size_t expected = 1; 
    /*Compare values*/
    std::array<std::size_t, 1> r{expected};
    TTS_ALL_EQUAL(*kwk::upper_bound(view, search), r);
    /*Compare with std implementation*/
    TTS_EQUAL ( (*kwk::upper_bound(view, search))[0]
              , (std::upper_bound(input.begin(), input.end(), search) - input.begin())
              );     
  }
  {
    int search = 18;
    std::size_t expected = 19; 
    /*Compare values*/
    std::array<std::size_t, 1> r{expected};
    TTS_ALL_EQUAL(*kwk::upper_bound(view, search), r);
    /*Compare with std implementation*/
    TTS_EQUAL ( (*kwk::upper_bound(view, search))[0]
              , (std::upper_bound(input.begin(), input.end(), search) - input.begin())
              );     
  }

  // Erroneous values
  {
    int search = 19;
    TTS_EQUAL(kwk::upper_bound(view, search), std::nullopt);
    TTS_EQUAL(std::upper_bound(input.begin(), input.end(), search), input.end());
  }
  {
    int search = 20;
    TTS_EQUAL(kwk::upper_bound(view, search), std::nullopt);
    TTS_EQUAL(std::upper_bound(input.begin(), input.end(), search), input.end());
  }
  {
    int search = 78456465;
    TTS_EQUAL(kwk::upper_bound(view, search), std::nullopt);
    TTS_EQUAL(std::upper_bound(input.begin(), input.end(), search), input.end());
  }
};


TTS_CASE("Check for kwk::upper_bound(In, value) 1D with function")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 3 - 20; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};
  auto func = [](auto const& search_, auto const& element_) { return search_ * 3 - 20 < element_; };

  // upper_bound returns the index of the first element above the specified value
  {
    int search = -78495;
    std::size_t expected = 0; 
    /*Compare values*/
    std::array<std::size_t, 1> r{expected};
    TTS_ALL_EQUAL(*kwk::upper_bound(view, search, func), r);
    /*Compare with std implementation*/
    TTS_EQUAL ( (*kwk::upper_bound(view, search, func))[0]
              , (std::upper_bound(input.begin(), input.end(), search, func) - input.begin())
              );     
  }
  {
    int search = -1;
    std::size_t expected = 0; 
    /*Compare values*/
    std::array<std::size_t, 1> r{expected};
    TTS_ALL_EQUAL(*kwk::upper_bound(view, search, func), r);
    /*Compare with std implementation*/
    TTS_EQUAL ( (*kwk::upper_bound(view, search, func))[0]
              , (std::upper_bound(input.begin(), input.end(), search, func) - input.begin())
              );     
  }
  {
    int search = 0;
    std::size_t expected = 1; 
    /*Compare values*/
    std::array<std::size_t, 1> r{expected};
    TTS_ALL_EQUAL(*kwk::upper_bound(view, search, func), r);
    /*Compare with std implementation*/
    TTS_EQUAL ( (*kwk::upper_bound(view, search, func))[0]
              , (std::upper_bound(input.begin(), input.end(), search, func) - input.begin())
              );     
  }
  {
    int search = 18;
    std::size_t expected = 19; 
    /*Compare values*/
    std::array<std::size_t, 1> r{expected};
    TTS_ALL_EQUAL(*kwk::upper_bound(view, search, func), r);
    /*Compare with std implementation*/
    TTS_EQUAL ( (*kwk::upper_bound(view, search, func))[0]
              , (std::upper_bound(input.begin(), input.end(), search, func) - input.begin())
              );     
  }

  // Erroneous values
  {
    int search = 19;
    TTS_EQUAL(kwk::upper_bound(view, search, func), std::nullopt);
    TTS_EQUAL(std::upper_bound(input.begin(), input.end(), search, func), input.end());
  }
  {
    int search = 20;
    TTS_EQUAL(kwk::upper_bound(view, search, func), std::nullopt);
    TTS_EQUAL(std::upper_bound(input.begin(), input.end(), search, func), input.end());
  }
  {
    int search = 78456465;
    TTS_EQUAL(kwk::upper_bound(view, search, func), std::nullopt);
    TTS_EQUAL(std::upper_bound(input.begin(), input.end(), search, func), input.end());
  }
};

TTS_CASE("Check for kwk::upper_bound 1D: size = 0")
{
  const std::size_t input_size = 0;
  std::vector<int> input; // will not work for an std::array of size 0
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 3 - 20; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};
  auto func = [](auto const& search_, auto const& element_) { return search_ * 3 - 20 < element_; };

  // Erroneous values
  {
    int search = -78495;
    TTS_EQUAL(kwk::upper_bound(view, search), std::nullopt);
    TTS_EQUAL(std::upper_bound(input.begin(), input.end(), search), input.end());
  }
  {
    int search = -1;
    TTS_EQUAL(kwk::upper_bound(view, search), std::nullopt);
    TTS_EQUAL(std::upper_bound(input.begin(), input.end(), search), input.end());
  }
  {
    int search = 0;
    TTS_EQUAL(kwk::upper_bound(view, search), std::nullopt);
    TTS_EQUAL(std::upper_bound(input.begin(), input.end(), search), input.end());
  }
  {
    int search = 18;
    TTS_EQUAL(kwk::upper_bound(view, search), std::nullopt);
    TTS_EQUAL(std::upper_bound(input.begin(), input.end(), search), input.end());
  }
  {
    int search = 19;
    TTS_EQUAL(kwk::upper_bound(view, search), std::nullopt);
    TTS_EQUAL(std::upper_bound(input.begin(), input.end(), search), input.end());
  }
  {
    int search = 20;
    TTS_EQUAL(kwk::upper_bound(view, search), std::nullopt);
    TTS_EQUAL(std::upper_bound(input.begin(), input.end(), search), input.end());
  }
  {
    int search = 78456465;
    TTS_EQUAL(kwk::upper_bound(view, search), std::nullopt);
    TTS_EQUAL(std::upper_bound(input.begin(), input.end(), search), input.end());
  }

  // Erroneous values, with function
  {
    int search = -78495;
    TTS_EQUAL(kwk::upper_bound(view, search, func), std::nullopt);
    TTS_EQUAL(std::upper_bound(input.begin(), input.end(), search, func), input.end());
  }
  {
    int search = -1;
    TTS_EQUAL(kwk::upper_bound(view, search, func), std::nullopt);
    TTS_EQUAL(std::upper_bound(input.begin(), input.end(), search, func), input.end());
  }
  {
    int search = 0;
    TTS_EQUAL(kwk::upper_bound(view, search, func), std::nullopt);
    TTS_EQUAL(std::upper_bound(input.begin(), input.end(), search, func), input.end());
  }
  {
    int search = 18;
    TTS_EQUAL(kwk::upper_bound(view, search, func), std::nullopt);
    TTS_EQUAL(std::upper_bound(input.begin(), input.end(), search, func), input.end());
  }
  {
    int search = 19;
    TTS_EQUAL(kwk::upper_bound(view, search, func), std::nullopt);
    TTS_EQUAL(std::upper_bound(input.begin(), input.end(), search, func), input.end());
  }
  {
    int search = 20;
    TTS_EQUAL(kwk::upper_bound(view, search, func), std::nullopt);
    TTS_EQUAL(std::upper_bound(input.begin(), input.end(), search, func), input.end());
  }
  {
    int search = 78456465;
    TTS_EQUAL(kwk::upper_bound(view, search, func), std::nullopt);
    TTS_EQUAL(std::upper_bound(input.begin(), input.end(), search, func), input.end());
  }
};

TTS_CASE("Check for kwk::binary_search(In, value) 1D")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 3; }
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  // Ensures that the return value is expected
  {
    bool expected = false;
    int search = -895;
    bool truth = kwk::binary_search(view, search);
    TTS_EQUAL(truth, expected); // Ensures that we did not make a mistake with the expected value
    TTS_EQUAL(truth, std::binary_search(input.begin(), input.end(), search));
  }
  {
    bool expected = false;
    int search = -1;
    bool truth = kwk::binary_search(view, search);
    TTS_EQUAL(truth, expected); // Ensures that we did not make a mistake with the expected value
    TTS_EQUAL(truth, std::binary_search(input.begin(), input.end(), search));
  }
  {
    bool expected = true;
    int search = 0;
    bool truth = kwk::binary_search(view, search);
    TTS_EQUAL(truth, expected); // Ensures that we did not make a mistake with the expected value
    TTS_EQUAL(truth, std::binary_search(input.begin(), input.end(), search));
  }
  {
    bool expected = false;
    int search = 18*3-1;
    bool truth = kwk::binary_search(view, search);
    TTS_EQUAL(truth, expected); // Ensures that we did not make a mistake with the expected value
    TTS_EQUAL(truth, std::binary_search(input.begin(), input.end(), search));
  }
  {
    bool expected = true;
    int search = 18*3;
    bool truth = kwk::binary_search(view, search);
    TTS_EQUAL(truth, expected); // Ensures that we did not make a mistake with the expected value
    TTS_EQUAL(truth, std::binary_search(input.begin(), input.end(), search));
  }
  {
    bool expected = false;
    int search = 18*3+1;
    bool truth = kwk::binary_search(view, search);
    TTS_EQUAL(truth, expected); // Ensures that we did not make a mistake with the expected value
    TTS_EQUAL(truth, std::binary_search(input.begin(), input.end(), search));
  }
  {
    bool expected = false;
    int search = 18*3+2;
    bool truth = kwk::binary_search(view, search);
    TTS_EQUAL(truth, expected); // Ensures that we did not make a mistake with the expected value
    TTS_EQUAL(truth, std::binary_search(input.begin(), input.end(), search));
  }
  {
    bool expected = true;
    int search = 19*3;
    bool truth = kwk::binary_search(view, search);
    TTS_EQUAL(truth, expected); // Ensures that we did not make a mistake with the expected value
    TTS_EQUAL(truth, std::binary_search(input.begin(), input.end(), search));
  }
  {
    bool expected = false;
    int search = 20*3;
    bool truth = kwk::binary_search(view, search);
    TTS_EQUAL(truth, expected); // Ensures that we did not make a mistake with the expected value
    TTS_EQUAL(truth, std::binary_search(input.begin(), input.end(), search));
  }
  {
    bool expected = false;
    int search = 2000*3;
    bool truth = kwk::binary_search(view, search);
    TTS_EQUAL(truth, expected); // Ensures that we did not make a mistake with the expected value
    TTS_EQUAL(truth, std::binary_search(input.begin(), input.end(), search));
  }
  {
    bool expected = false;
    int search = 2000*3+1;
    bool truth = kwk::binary_search(view, search);
    TTS_EQUAL(truth, expected); // Ensures that we did not make a mistake with the expected value
    TTS_EQUAL(truth, std::binary_search(input.begin(), input.end(), search));
  }
};

TTS_CASE("Check for kwk::binary_search(In, value) 1D with function v1")
{
  const std::size_t input_size = 20;
  std::array<int, input_size> input;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 3; } //  * 3 - 20
  auto view = kwk::view{kwk::source = input, kwk::of_size(input_size)};

  auto func = [](auto const& search_, auto const& element_) { return search_ < element_; };



    // Ensures that the return value is expected
  {
    bool expected = false;
    int search = -895;
    bool truth = kwk::binary_search(view, search, func);
    TTS_EQUAL(truth, expected); // Ensures that we did not make a mistake with the expected value
    TTS_EQUAL(truth, std::binary_search(input.begin(), input.end(), search, func));
  }
  {
    bool expected = false;
    int search = -1;
    bool truth = kwk::binary_search(view, search, func);
    TTS_EQUAL(truth, expected); // Ensures that we did not make a mistake with the expected value
    TTS_EQUAL(truth, std::binary_search(input.begin(), input.end(), search, func));
  }
  {
    bool expected = true;
    int search = 0;
    bool truth = kwk::binary_search(view, search, func);
    TTS_EQUAL(truth, expected); // Ensures that we did not make a mistake with the expected value
    TTS_EQUAL(truth, std::binary_search(input.begin(), input.end(), search, func));
  }
  {
    bool expected = false;
    int search = 18*3-1;
    bool truth = kwk::binary_search(view, search, func);
    TTS_EQUAL(truth, expected); // Ensures that we did not make a mistake with the expected value
    TTS_EQUAL(truth, std::binary_search(input.begin(), input.end(), search, func));
  }
  {
    bool expected = true;
    int search = 18*3;
    bool truth = kwk::binary_search(view, search, func);
    TTS_EQUAL(truth, expected); // Ensures that we did not make a mistake with the expected value
    TTS_EQUAL(truth, std::binary_search(input.begin(), input.end(), search, func));
  }
  {
    bool expected = false;
    int search = 18*3+1;
    bool truth = kwk::binary_search(view, search, func);
    TTS_EQUAL(truth, expected); // Ensures that we did not make a mistake with the expected value
    TTS_EQUAL(truth, std::binary_search(input.begin(), input.end(), search, func));
  }
  {
    bool expected = false;
    int search = 18*3+2;
    bool truth = kwk::binary_search(view, search, func);
    TTS_EQUAL(truth, expected); // Ensures that we did not make a mistake with the expected value
    TTS_EQUAL(truth, std::binary_search(input.begin(), input.end(), search, func));
  }
  {
    bool expected = true;
    int search = 19*3;
    bool truth = kwk::binary_search(view, search, func);
    TTS_EQUAL(truth, expected); // Ensures that we did not make a mistake with the expected value
    TTS_EQUAL(truth, std::binary_search(input.begin(), input.end(), search, func));
  }
  {
    bool expected = false;
    int search = 20*3;
    bool truth = kwk::binary_search(view, search, func);
    TTS_EQUAL(truth, expected); // Ensures that we did not make a mistake with the expected value
    TTS_EQUAL(truth, std::binary_search(input.begin(), input.end(), search, func));
  }
  {
    bool expected = false;
    int search = 2000*3;
    bool truth = kwk::binary_search(view, search, func);
    TTS_EQUAL(truth, expected); // Ensures that we did not make a mistake with the expected value
    TTS_EQUAL(truth, std::binary_search(input.begin(), input.end(), search, func));
  }
  {
    bool expected = false;
    int search = 2000*3+1;
    bool truth = kwk::binary_search(view, search, func);
    TTS_EQUAL(truth, expected); // Ensures that we did not make a mistake with the expected value
    TTS_EQUAL(truth, std::binary_search(input.begin(), input.end(), search, func));
  }
};
