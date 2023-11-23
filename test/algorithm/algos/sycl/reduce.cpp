//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/reduce.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include "../generic/reduce.hpp"

#include <kwk/context/sycl/context.hpp>

// TODO: ne marche pas, comprendre pourquoi

TTS_CASE("Check for kwk::reduce(in) 1D")
{
  kwk::test::reduce_in_1D(kwk::sycl::default_context);
};

TTS_CASE("Check for kwk::reduce(in) 1D - larger")
{
  kwk::test::reduce_in_1D_larger(kwk::sycl::default_context);
};

TTS_CASE("Check for kwk::reduce(in) 2D")
{
  kwk::test::reduce_in_2D(kwk::sycl::default_context);
};

TTS_CASE("Check for kwk::reduce(in) 3D")
{
  kwk::test::reduce_in_3D(kwk::sycl::default_context);
};

TTS_CASE("Check for kwk::reduce(in) 4D")
{
  kwk::test::reduce_in_4D(kwk::sycl::default_context);
};

TTS_CASE("Check for kwk::reduce(in, func) 1D")
{
  kwk::test::reduce_in_func_1D(kwk::sycl::default_context);
};

TTS_CASE("Check for kwk::reduce(in, func) 2D")
{
  kwk::test::reduce_in_func_1D(kwk::sycl::default_context);
};

TTS_CASE("Check for kwk::reduce(in, func) 3D")
{
  kwk::test::reduce_in_func_3D(kwk::sycl::default_context);
};

TTS_CASE("Check for kwk::reduce(in, func) 4D")
{
  kwk::test::reduce_in_func_4D(kwk::sycl::default_context);
};

TTS_CASE("Check for float kwk::reduce(in, func)")
{
  kwk::test::reduce_in_func_float(kwk::sycl::default_context);
};

TTS_CASE("Check for kwk::reduce(in, func, init) 1D")
{
  kwk::test::reduce_in_func_init_1D(kwk::sycl::default_context);
};

TTS_CASE("Check for kwk::reduce(in, func, init) 2D")
{
  kwk::test::reduce_in_func_init_2D(kwk::sycl::default_context);
};

TTS_CASE("Check for kwk::reduce(in, func, init) 3D")
{
  kwk::test::reduce_in_func_init_3D(kwk::sycl::default_context);
};

TTS_CASE("Check for kwk::reduce(in, func, init) 4D")
{
  kwk::test::reduce_in_func_init_4D(kwk::sycl::default_context);
};