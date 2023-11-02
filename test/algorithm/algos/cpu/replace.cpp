//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/replace.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include "../generic/replace.hpp"

TTS_CASE("Check for kwk::replace(value, new_value) 1D")
{
  kwk::test::replace_value_new_value_1D(kwk::cpu);
};

TTS_CASE("Check for kwk::replace(value, new_value) 2D")
{
  kwk::test::replace_value_new_value_2D(kwk::cpu);
};

TTS_CASE("Check for kwk::replace(value, new_value) 3D")
{
  kwk::test::replace_value_new_value_3D(kwk::cpu);
};

TTS_CASE("Check for kwk::replace(value, new_value) 4D")
{
  kwk::test::replace_value_new_value_4D(kwk::cpu);
};

TTS_CASE("Check for kwk::replace_if(func, new_value) 1D")
{
  kwk::test::replace_if_func_new_value_1D(kwk::cpu);
};

TTS_CASE("Check for kwk::replace_if(func, new_value) 2D")
{
  kwk::test::replace_if_func_new_value_2D(kwk::cpu);
};

TTS_CASE("Check for kwk::replace_if(func, new_value) 3D")
{
  kwk::test::replace_if_func_new_value_3D(kwk::cpu);
};

TTS_CASE("Check for kwk::replace_if(func, new_value) 4D")
{
  kwk::test::replace_if_func_new_value_4D(kwk::cpu);
};

TTS_CASE("Check for kwk::replace_if(func, new_value) 4D smaller view - with CPU context")
{
  kwk::test::replace_if_func_new_value_4D_smaller_view(kwk::cpu);
};
