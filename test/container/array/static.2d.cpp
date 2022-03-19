//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kwk/container/array.hpp>

TTS_CASE( "Automatic storage, constexpr 2D shape array - Basic properties" )
{
  kwk::array<float,kwk::extent[9][3]> a;
  TTS_EQUAL( sizeof(a), a.size()*sizeof(float)  );
  TTS_EQUAL( a.size() , 27                      );
  TTS_EQUAL( a.shape(), kwk::of_shape(9,3)      );
};

TTS_CASE( "Automatic storage, constexpr 2D shape array - Copy constructor" )
{
  kwk::array<float,kwk::extent[9][3]> ref;

  float k = 1.f;
  for(auto& e : ref) e = k++;

  kwk::array<float,kwk::extent[9][3]> a = ref;
  TTS_EXPECT( std::equal(ref.begin(),ref.end(),a.begin()) );
};

TTS_CASE( "Automatic storage, constexpr 2D shape array - Move constructor" )
{
  kwk::array<float,kwk::extent[9][3]> ref, ref2;

  float k = 1.f;
  for(auto& e : ref) e = k++;
  ref2 = ref;

  kwk::array<float,kwk::extent[9][3]> a = std::move(ref2);
  TTS_EXPECT( std::equal(ref.begin(),ref.end(),a.begin()) );
};

TTS_CASE( "Automatic storage, constexpr 2D shape array - Assignment operator" )
{
  kwk::array<float,kwk::extent[9][3]> a, ref;

  float k = 1.f;
  for(auto& e : ref) e = k++;

  a = ref;
  TTS_EXPECT( std::equal(ref.begin(),ref.end(),a.begin()) );
};

TTS_CASE( "Automatic storage, constexpr 2D shape array - Move constructor" )
{
  kwk::array<float,kwk::extent[9][3]> a, ref, ref2;

  float k = 1.f;
  for(auto& e : ref) e = k++;
  ref2 = ref;

  a = std::move(ref2);
  TTS_EXPECT( std::equal(ref.begin(),ref.end(),a.begin()) );
};
