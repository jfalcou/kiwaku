//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/detail/raberu.hpp>
#include <kwk/utility/container/shape.hpp>

TTS_CASE("Check dynamic 1D slicing")
{
  using namespace kwk::literals;

  auto shape = kwk::of_size(27);

  TTS_EQUAL(shape(kwk::_)                           , shape           );
  TTS_EQUAL(shape(3)                                , kwk::of_size(1) );
  TTS_EQUAL(get<0>(shape(3).descriptor)             , 1U              );
  TTS_EQUAL(shape(kwk::from(7))                     , kwk::of_size(20));
  TTS_EQUAL(shape(kwk::from(7_c))                   , kwk::of_size(20));
  TTS_EQUAL(shape(kwk::to(11))                      , kwk::of_size(11));
  TTS_EQUAL(shape(kwk::to(11_c))                    , kwk::of_size(11));
};

TTS_CASE("Check static 1D slicing")
{
  using namespace kwk::literals;

  auto shape = kwk::of_size(27_c);

  TTS_EQUAL(shape(kwk::_)                           , shape           );
  TTS_EQUAL(get<0>(shape(kwk::_).descriptor)        , 27U             );
  TTS_EQUAL(shape(3)                                , kwk::of_size(1) );
  TTS_EQUAL(get<0>(shape(3).descriptor)             , 1U              );
  TTS_EQUAL(shape(kwk::from(7))                     , kwk::of_size(20));
  TTS_EQUAL(shape(kwk::from(7_c))                   , kwk::of_size(20));
  TTS_EQUAL(get<0>(shape(kwk::from(7_c)).descriptor), 20U             );
  TTS_EQUAL(shape(kwk::to(11))                      , kwk::of_size(11));
  TTS_EQUAL(shape(kwk::to(11_c))                    , kwk::of_size(11));
  TTS_EQUAL(get<0>(shape(kwk::to(11_c)).descriptor) , 11U             );
};
