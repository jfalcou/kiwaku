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

TTS_CASE( "Dynamic storage, constexpr 2D shape array - Basic properties" )
{
  kwk::array<float,kwk::extent[9][3], kwk::heap_> a;
  TTS_EQUAL( sizeof(a), sizeof(void*) + sizeof(std::ptrdiff_t));
  TTS_EQUAL( a.size() , 27                                    );
  TTS_EQUAL( a.shape(), kwk::of_shape(9,3)                    );
};

TTS_CASE( "Dynamic storage, constexpr 2D shape array - Copy constructor" )
{
  kwk::array<float,kwk::extent[9][3],kwk::heap_> ref;

  float k = 1.f;
  for(auto& e : ref) e = k++;

  kwk::array<float,kwk::extent[9][3],kwk::heap_> a = ref;
  TTS_EXPECT( std::equal(ref.begin(),ref.end(),a.begin()) );
};

TTS_CASE( "Dynamic storage, constexpr 2D shape array - Move constructor" )
{
  kwk::array<float,kwk::extent[9][3],kwk::heap_> ref, ref2;

  float k = 1.f;
  for(auto& e : ref) e = k++;

  k = 1.f;
  for(auto& e : ref2) e = k++;

  auto ref_data = ref2.data();
  kwk::array<float,kwk::extent[9][3],kwk::heap_> a = std::move(ref2);

  TTS_EXPECT( std::equal(ref.begin(),ref.end(),a.begin()) );
  TTS_EQUAL( a.data()     , ref_data  );
};

TTS_CASE( "Dynamic storage, constexpr 2D shape array - Assignment operator" )
{
  kwk::array<float,kwk::extent[9][3],kwk::heap_> a, ref;

  float k = 1.f;
  for(auto& e : ref) e = k++;

  a = ref;
  TTS_EXPECT( std::equal(ref.begin(),ref.end(),a.begin()) );
};

TTS_CASE( "Dynamic storage, constexpr 2D shape array - Move assignment operator" )
{
  kwk::array<float,kwk::extent[9][3],kwk::heap_> a, ref, ref2;

  float k = 1.f;
  for(auto& e : ref) e = k++;

  k = 1.f;
  for(auto& e : ref2) e = k++;

  auto ref_data = ref2.data();

  a = std::move(ref2);
  TTS_EXPECT( std::equal(ref.begin(),ref.end(),a.begin()) );

  TTS_EQUAL(a.data(), ref_data);
};

TTS_CASE( "Dynamic storage, dynamic 2D shape array - Basic properties" )
{
  kwk::array<float,kwk::_2D> a;
  TTS_EQUAL( sizeof(a), sizeof(a.as_view())+sizeof(a.size()));
  TTS_EQUAL( a.size() , 0                                   );
  TTS_EQUAL( a.shape(), kwk::of_shape(0)                    );
};

TTS_CASE( "Dynamic storage, dynamic 2D shape array - Copy constructor" )
{
  kwk::array<float,kwk::_2D> ref(kwk::of_shape(9)), ref2(kwk::of_shape(9));

  float k = 1.f;
  for(auto& e : ref) e = k++;

  kwk::array<float,kwk::_2D> a = ref;
  TTS_EXPECT( std::equal(ref.begin(),ref.end(),a.begin()) );
};

TTS_CASE( "Dynamic storage, dynamic 2D shape array - Move constructor" )
{
  kwk::array<float,kwk::_2D> ref(kwk::of_shape(9)), ref2(kwk::of_shape(9));

  float k = 1.f;
  for(auto& e : ref) e = k++;

  k = 1.f;
  for(auto& e : ref2) e = k++;

  auto ref_data = ref2.data();

  kwk::array<float,kwk::_2D> a = std::move(ref2);
  TTS_EXPECT( std::equal(ref.begin(),ref.end(),a.begin()) );
  TTS_EQUAL( a.data(), ref_data );
};

TTS_CASE( "Dynamic storage, dynamic 2D shape array - Assignment operator" )
{
  kwk::array<float,kwk::_2D> ref(kwk::of_shape(9)), ref2(kwk::of_shape(9)), a;

  float k = 1.f;
  for(auto& e : ref) e = k++;

  a = ref;
  TTS_EXPECT( std::equal(ref.begin(),ref.end(),a.begin()) );
};

TTS_CASE( "Dynamic storage, dynamic 2D shape array - Move assignment operator" )
{
  kwk::array<float,kwk::_2D> ref(kwk::of_shape(9)), ref2(kwk::of_shape(9)), a;

  float k = 1.f;
  for(auto& e : ref) e = k++;

  k = 1.f;
  for(auto& e : ref2) e = k++;

  auto ref_data = ref2.data();

  a = std::move(ref2);
  TTS_EXPECT( std::equal(ref.begin(),ref.end(),a.begin()) );
  TTS_EQUAL( a.data(), ref_data );
};
