//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/container/array.hpp>
#include <tts/tests/relation.hpp>
#include <tts/tests/basic.hpp>

TTS_CASE( "Dynamic storage, constexpr 1D shape array - Basic properties" )
{
  kwk::array<float,kwk::_1D(9), kwk::dynamic_> a;
  TTS_EQUAL( sizeof(a), sizeof(void*)   );
  TTS_EQUAL( a.size() , 9ULL            );
  TTS_EQUAL( a.count(), 9               );
  TTS_EQUAL( a.shape(), kwk::of_shape(9));
}

TTS_CASE( "Dynamic storage, constexpr 1D shape array - Copy constructor" )
{
  kwk::array<float,kwk::_1D(9),kwk::dynamic_> ref;

  float k = 1.f;
  for(auto& e : ref) e = k++;

  kwk::array<float,kwk::_1D(9),kwk::dynamic_> a = ref;
  TTS_EXPECT( std::equal(ref.begin(),ref.end(),a.begin()) );
}

TTS_CASE( "Dynamic storage, constexpr 1D shape array - Move constructor" )
{
  kwk::array<float,kwk::_1D(9),kwk::dynamic_> ref, ref2;

  float k = 1.f;
  for(auto& e : ref) e = k++;

  k = 1.f;
  for(auto& e : ref2) e = k++;

  kwk::array<float,kwk::_1D(9),kwk::dynamic_> a = std::move(ref2);
  TTS_EXPECT( std::equal(ref.begin(),ref.end(),a.begin()) );
  TTS_EQUAL( ref2.begin(), ref2.end()  );
}

TTS_CASE( "Dynamic storage, constexpr 1D shape array - Assignment operator" )
{
  kwk::array<float,kwk::_1D(9),kwk::dynamic_> a, ref;

  float k = 1.f;
  for(auto& e : ref) e = k++;

  a = ref;
  TTS_EXPECT( std::equal(ref.begin(),ref.end(),a.begin()) );
}

TTS_CASE( "Dynamic storage, constexpr 1D shape array - Move assignment operator" )
{
  kwk::array<float,kwk::_1D(9),kwk::dynamic_> a, ref, ref2;

  float k = 1.f;
  for(auto& e : ref) e = k++;

  k = 1.f;
  for(auto& e : ref2) e = k++;

  a = std::move(ref2);
  TTS_EXPECT( std::equal(ref.begin(),ref.end(),a.begin()) );
  TTS_EQUAL( ref2.begin(), ref2.end()  );
}

TTS_CASE( "Dynamic storage, dynamic 1D shape array - Basic properties" )
{
  kwk::array<float,kwk::_1D> a;
  TTS_EQUAL( sizeof(a), sizeof(void*)+sizeof(a.size())  );
  TTS_EQUAL( a.size() , 0ULL                            );
  TTS_EQUAL( a.count(), 0                               );
  TTS_EQUAL( a.shape(), kwk::of_shape(0)                );
}

TTS_CASE( "Dynamic storage, constexpr 1D shape array - Copy constructor" )
{
  kwk::array<float,kwk::_1D> ref(kwk::_1D(9)), ref2(kwk::_1D(9));

  float k = 1.f;
  for(auto& e : ref) e = k++;

  kwk::array<float,kwk::_1D> a = ref;
  TTS_EXPECT( std::equal(ref.begin(),ref.end(),a.begin()) );
}

TTS_CASE( "Dynamic storage, constexpr 1D shape array - Move constructor" )
{
  kwk::array<float,kwk::_1D> ref(kwk::_1D(9)), ref2(kwk::_1D(9));

  float k = 1.f;
  for(auto& e : ref) e = k++;

  k = 1.f;
  for(auto& e : ref2) e = k++;

  kwk::array<float,kwk::_1D> a = std::move(ref2);
  TTS_EXPECT( std::equal(ref.begin(),ref.end(),a.begin()) );
  TTS_EQUAL( ref2.begin(), ref2.end()  );
  TTS_EQUAL( ref2.size() , 0ULL        );
  TTS_EQUAL( ref2.count(), 0LL         );
}

TTS_CASE( "Dynamic storage, constexpr 1D shape array - Assignment operator" )
{
  kwk::array<float,kwk::_1D> ref(kwk::_1D(9)), ref2(kwk::_1D(9)), a;

  float k = 1.f;
  for(auto& e : ref) e = k++;

  a = ref;
  TTS_EXPECT( std::equal(ref.begin(),ref.end(),a.begin()) );
}

TTS_CASE( "Dynamic storage, constexpr 1D shape array - Move assignment operator" )
{
  kwk::array<float,kwk::_1D> ref(kwk::_1D(9)), ref2(kwk::_1D(9)), a;

  float k = 1.f;
  for(auto& e : ref) e = k++;

  k = 1.f;
  for(auto& e : ref2) e = k++;

  a = std::move(ref2);
  TTS_EXPECT( std::equal(ref.begin(),ref.end(),a.begin()) );
  TTS_EQUAL( ref2.begin(), ref2.end()  );
  TTS_EQUAL( ref2.size() , 0ULL        );
  TTS_EQUAL( ref2.count(), 0LL         );
}
