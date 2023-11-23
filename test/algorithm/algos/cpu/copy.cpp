//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#include <cstdlib>
#include <kwk/algorithm/algos/copy.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

#include <kwk/context/sycl/context.hpp>
#include "../generic/copy.hpp"

TTS_CASE("Check for kwk::copy(out, in) 1D")
{
  kwk::test::copy_out_in_1D(kwk::cpu);
};

TTS_CASE("Check for kwk::copy(out, in) 2D")
{
  kwk::test::copy_out_in_2D(kwk::cpu);
};

TTS_CASE("Check for kwk::copy(out, in) 3D")
{
  kwk::test::copy_out_in_3D(kwk::cpu);
};

TTS_CASE("Check for kwk::copy(out, in) 4D")
{
  kwk::test::copy_out_in_4D(kwk::cpu);
};

TTS_CASE("Check for double kwk::copy(out, in) 2D")
{
  kwk::test::copy_out_in_2D_double(kwk::cpu);
};

TTS_CASE("Check for kwk::copy_if(func, out, in) 1D")
{
  kwk::test::copy_if_func_out_in_1D(kwk::cpu);
};

TTS_CASE("Check for kwk::copy_if(func, out, in) 2D")
{
  kwk::test::copy_if_func_out_in_2D(kwk::cpu);
};

TTS_CASE("Check for kwk::copy_if(func, out, in) 3D")
{
  kwk::test::copy_if_func_out_in_3D(kwk::cpu);
};


TTS_CASE("Check for kwk::copy_if(func, out, in) 4D")
{
  kwk::test::copy_if_func_out_in_4D(kwk::cpu);
};