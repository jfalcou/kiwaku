//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/size_t.hpp>

struct integral_pairs
{
  using i8 = std::uint8_t;
  using i16 = std::uint16_t;
  using i32 = std::uint32_t;
  using i64 = std::uint64_t;
  using types_list = 
  tts::types
  <
      kumi::tuple<i8, i8>, kumi::tuple<i8, i16>, kumi::tuple<i8, i32>, kumi::tuple<i8, i64>
    , kumi::tuple<i16, i8>, kumi::tuple<i16, i16>, kumi::tuple<i16, i32>, kumi::tuple<i16, i64>
    , kumi::tuple<i32, i8>, kumi::tuple<i32, i16>, kumi::tuple<i32, i32>, kumi::tuple<i32, i64>
    , kumi::tuple<i64, i8>, kumi::tuple<i64, i16>, kumi::tuple<i64, i32>, kumi::tuple<i64, i64>
  >;
};

TTS_CASE_TPL( "Check comparison between kwk::size_t", integral_pairs )
<typename T>( tts::type<T> )
{
  using T1 = kumi::element_t<0, T>;
  using T2 = kumi::element_t<1, T>;
  auto t1 = kwk::size_t<T1>(54);
  auto t2 = kwk::size_t<T2>(97);
  TTS_EQUAL( t1, t1 );
  TTS_NOT_EQUAL( t1, t2 );
  TTS_LESS_EQUAL( t1, t1 );
  TTS_LESS_EQUAL( t1, t2 );
  TTS_GREATER_EQUAL( t1, t1 );
  TTS_GREATER_EQUAL( t2, t1 );
  TTS_LESS( t1, t2 );
  TTS_GREATER( t2, t1 );
};

