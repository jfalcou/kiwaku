//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/transform.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include "../transform.hpp"

#include <kwk/context/sycl/context.hpp>

TTS_CASE("Check for kwk::transform(value, new_value) 1D - SYCL context")
{
  kwk::test::transform_value_new_value_1D(kwk::sycl::sycl_context);
};

TTS_CASE("Check for kwk::transform(value, new_value) 2D - SYCL context")
{
  kwk::test::transform_value_new_value_2D(kwk::sycl::sycl_context);
};

TTS_CASE("Check for kwk::transform(value, new_value) 3D - SYCL context")
{
  kwk::test::transform_value_new_value_3D(kwk::sycl::sycl_context);
};

TTS_CASE("Check for kwk::transform(value, new_value) 4D - SYCL context")
{
  kwk::test::transform_value_new_value_4D(kwk::sycl::sycl_context);
};

// TTS_CASE("Check for kwk::transform(value, new_value) 1D")
// {
//   // Pas possible de faire un test générique en conservant les particularités du cpu
//   kwk::test::transform_value_new_value_1D(kwk::sycl::sycl_context);
//   //  error: 'int &' cannot be used as the type of a kernel parameter
//   //     count++;
//   //     ^
// };

// On fait donc :
// test/algorithm/algos/<les algos.cpp sans contexte>
// et des test/algorithm/algos/cpu/  sycl/ etc