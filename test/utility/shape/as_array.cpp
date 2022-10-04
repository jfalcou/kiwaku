//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/utility/container/shape.hpp>

template<typename T, std::size_t N>
using type = std::array<T,N>;

TTS_CASE( "Convert a 1D shape to std::array" )
{
  using namespace kwk::literals;
  TTS_EQUAL( kwk::of_size(9     ).as_array(), (type<int,1>{9})              );
  TTS_EQUAL( kwk::of_size(9_c   ).as_array(), (type<unsigned char,1>{9})    );
  TTS_EQUAL( kwk::of_size(390_c ).as_array(), (type<unsigned short,1>{390}) );
};

TTS_CASE( "Convert a 2D shape to std::array" )
{
  using namespace kwk::literals;
  TTS_EQUAL( kwk::of_size(9    , 7  ).as_array(), (type<int,2>{9,7})              );
  TTS_EQUAL( kwk::of_size(9_c  , 7_c).as_array(), (type<unsigned char,2>{9,7})    );
  TTS_EQUAL( kwk::of_size(390_c, 7_c).as_array(), (type<unsigned short,2>{390,7}) );
};

TTS_CASE( "Convert a 3D shape to std::array" )
{
  using namespace kwk::literals;
  TTS_EQUAL( kwk::of_size(9    , 7  , 11  ).as_array(), (type<int,3>{9,7,11})              );
  TTS_EQUAL( kwk::of_size(9_c  , 7_c, 11_c).as_array(), (type<unsigned char,3>{9,7,11})    );
  TTS_EQUAL( kwk::of_size(390_c, 7_c, 11_c).as_array(), (type<unsigned short,3>{390,7,11}) );
};

TTS_CASE( "Convert a 4D shape to std::array" )
{
  using namespace kwk::literals;
  TTS_EQUAL( kwk::of_size(9    , 7  , 11  , 32        ).as_array(), (type<int,4>{9,7,11,32})            );
  TTS_EQUAL( kwk::of_size(9_c  , 7_c, 11_c, ' '       ).as_array(), (type<unsigned char,4>{9,7,11,32})  );
  TTS_EQUAL( kwk::of_size(390_c, 7_c, 11_c, short{32} ).as_array(), (type<unsigned short,4>{390,7,11,32}));
};
