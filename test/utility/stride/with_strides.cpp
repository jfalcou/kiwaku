//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/container/stride.hpp>
#include <kwk/utility/fixed.hpp>

using namespace kwk::literals;
auto in = kumi::tuple{1_c,2_c,9};

void test_strides(auto values)
{
  kumi::for_each( [](auto m)
  {
    auto the_stride = kwk::with_strides(m);
    TTS_EQUAL(the_stride.size(), m.size());

    kumi::for_each( [&](auto o, auto e) { TTS_EQUAL(o,e); }, the_stride, m );

    if constexpr( std::same_as<decltype(1_c), kumi::element_t<0,decltype(m)>>)
      TTS_CONSTEXPR_EXPECT( the_stride.is_unit );
    else
      TTS_CONSTEXPR_EXPECT_NOT( the_stride.is_unit );
  }
  , values
  );
}

TTS_CASE( "Building a 1D stride using with_strides" )
{
  auto values = kumi::cartesian_product(in);
  test_strides(values);
};

TTS_CASE( "Building a 2D stride using with_strides" )
{
  auto values = kumi::cartesian_product(in,in);
  test_strides(values);
};

TTS_CASE( "Building a 3D stride using with_strides" )
{
  auto values = kumi::cartesian_product(in,in,in);
  test_strides(values);
};

TTS_CASE( "Building a 4D stride using with_strides" )
{
  auto values = kumi::cartesian_product(in,in,in,in);
  test_strides(values);
};
