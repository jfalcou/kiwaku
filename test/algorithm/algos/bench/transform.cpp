//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

#include <kwk/algorithm/algos/transform.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include "chrono.hpp"

#include <kwk/context/sycl/context.hpp>

TTS_CASE("Check for kwk::transform(value, new_value) 1D - SYCL context")
{
  int data[2];
  double res[2];
  double vdata[2] = {1, 0.5};

  fill_data(data, kwk::of_size(2), true);

  auto d = kwk::view{kwk::source = data, kwk::of_size(2)};
  auto v = kwk::view{kwk::source = res, kwk::of_size(2)};

  ::kwk::transform(kwk::sycl::default_context, [&](auto e) { return 1.0/(1.0+e); }, v, d);

  TTS_ALL_EQUAL(res, vdata);
};

// On fait donc :
// test/algorithm/algos/<les algos.cpp sans contexte>
// et des test/algorithm/algos/cpu/  sycl/ etc