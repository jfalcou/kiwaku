//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <cstdlib>
#include <kwk/algorithm/views/reshape.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

// waiting for shape constraint

// TTS_CASE("Check for kwk::reshape(in, name) 1D")
// {
//   int data[2];

//   fill_data(data, kwk::of_size(2), false);

//   auto d = kwk::view{kwk::source = data};
//   auto e = kwk::reshape(d, kwk::of_size(3));

//   std::cout << e.shape() << std::endl;
//   std::cout << d << std::endl;
//   std::cout << e << std::endl;

//   TTS_EQUAL(e.shape(), kwk::of_size(3));
// };