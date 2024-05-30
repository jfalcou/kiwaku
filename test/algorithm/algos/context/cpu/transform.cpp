//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/transform.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include "../generic/transform.hpp"

TTS_CASE("Check for kwk::transform(value, new_value) 1D - CPU context")
{
  kwk::test::transform_value_new_value_1D(kwk::cpu);
};

TTS_CASE("Check for kwk::transform(value, new_value) 2D - CPU context")
{
  kwk::test::transform_value_new_value_2D(kwk::cpu);
};

TTS_CASE("Check for kwk::transform(value, new_value) 3D - CPU context")
{
  kwk::test::transform_value_new_value_3D(kwk::cpu);
};

TTS_CASE("Check for kwk::transform(value, new_value) 4D - CPU context")
{
  kwk::test::transform_value_new_value_4D(kwk::cpu);
};
