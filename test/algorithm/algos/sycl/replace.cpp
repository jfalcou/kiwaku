//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#include "../generic/replace.hpp"

#include <kwk/context/sycl/context.hpp>

TTS_CASE("Check for kwk::replace(value, new_value) 1D")
{
  kwk::test::replace_value_new_value_1D(kwk::sycl::default_context);
};

TTS_CASE("Check for kwk::replace(value, new_value) 2D")
{
  kwk::test::replace_value_new_value_2D(kwk::sycl::default_context);
};

TTS_CASE("Check for kwk::replace(value, new_value) 3D")
{
  kwk::test::replace_value_new_value_3D(kwk::sycl::default_context);
};

TTS_CASE("Check for kwk::replace(value, new_value) 4D")
{
  kwk::test::replace_value_new_value_4D(kwk::sycl::default_context);
};

TTS_CASE("Check for kwk::replace_if(func, new_value) 1D")
{
  kwk::test::replace_if_func_new_value_1D(kwk::sycl::default_context);
};

TTS_CASE("Check for kwk::replace_if(func, new_value) 2D")
{
  kwk::test::replace_if_func_new_value_2D(kwk::sycl::default_context);
};

TTS_CASE("Check for kwk::replace_if(func, new_value) 3D")
{
  kwk::test::replace_if_func_new_value_3D(kwk::sycl::default_context);
};

TTS_CASE("Check for kwk::replace_if(func, new_value) 4D")
{
  kwk::test::replace_if_func_new_value_4D(kwk::sycl::default_context);
};

TTS_CASE("Check for kwk::replace_if(func, new_value) 4D smaller view - with CPU context")
{
  kwk::test::replace_if_func_new_value_4D_smaller_view(kwk::sycl::default_context);
};
