//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/container/shape.hpp>

TTS_CASE( "swap behavior on 1D shape" )
{
  auto first      = kwk::of_size(4) , second     = kwk::of_size(2);
  auto old_first  = kwk::of_size(4) , old_second = kwk::of_size(2);

  first.swap(second);

  TTS_EQUAL(first , old_second);
  TTS_EQUAL(second, old_first );
};

TTS_CASE( "swap behavior on 2D shape" )
{
  auto first      = kwk::of_size(4,7) , second     = kwk::of_size(2,9);
  auto old_first  = kwk::of_size(4,7) , old_second = kwk::of_size(2,9);

  first.swap(second);

  TTS_EQUAL(first , old_second);
  TTS_EQUAL(second, old_first );
};

TTS_CASE( "swap behavior on 3D shape" )
{
  auto first      = kwk::of_size(4,7,9) , second     = kwk::of_size(2,9,3);
  auto old_first  = kwk::of_size(4,7,9) , old_second = kwk::of_size(2,9,3);

  first.swap(second);

  TTS_EQUAL(first , old_second);
  TTS_EQUAL(second, old_first );
};

TTS_CASE( "swap behavior on 4D shape" )
{
  auto first      = kwk::of_size(4,7,9,8) , second     = kwk::of_size(2,9,3,6);
  auto old_first  = kwk::of_size(4,7,9,8) , old_second = kwk::of_size(2,9,3,6);

  first.swap(second);

  TTS_EQUAL(first , old_second);
  TTS_EQUAL(second, old_first );
};
