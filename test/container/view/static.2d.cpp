//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/container/view.hpp>
#include <kiwaku/algorithm/all.hpp>
#include <algorithm>
#include <vector>

TTS_CASE( "Build a 2D view with constexpr shape settings from a C array" )
{
  float ref[15] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

  kwk::view<float,kwk::extent[3][5]> view(ref);

  TTS_EQUAL( sizeof(view), sizeof(void*)      );
  TTS_EQUAL( view.size() , 15ULL              );
  TTS_EQUAL( view.count(), 15                 );
  TTS_EQUAL( view.shape(), kwk::of_shape(3,5) );

  TTS_EXPECT( (std::equal( view.begin(), view.end(), &ref[0])) );
  TTS_EXPECT( (kwk::all( [&](auto i, auto j) { return view(i,j) == ref[i+3*j]; }, view.shape())) );
}

TTS_CASE( "Build a 2D view with constexpr shape settings from a pointer" )
{
  float ref[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

  kwk::view<float,kwk::extent[3][5]> view(&ref[0]);

  TTS_EQUAL( sizeof(view), sizeof(void*)      );
  TTS_EQUAL( view.size() , 15ULL              );
  TTS_EQUAL( view.count(), 15                 );
  TTS_EQUAL( view.shape(), kwk::of_shape(3,5) );

  TTS_EXPECT( (std::equal( view.begin(), view.end(), &ref[0])) );
  TTS_EXPECT( (kwk::all( [&](auto i, auto j) { return view(i,j) == ref[i+3*j]; }, view.shape())) );
}

TTS_CASE( "Build a 2D view with constexpr shape settings from a pointer to const" )
{
  float const ref[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

  kwk::view<float const,kwk::extent[3][5]> view(&ref[0]);

  TTS_EQUAL( sizeof(view), sizeof(void*)      );
  TTS_EQUAL( view.size() , 15ULL              );
  TTS_EQUAL( view.count(), 15                 );
  TTS_EQUAL( view.shape(), kwk::of_shape(3,5) );

  TTS_EXPECT( (std::equal( view.begin(), view.end(), &ref[0])) );
  TTS_EXPECT( (kwk::all( [&](auto i, auto j) { return view(i,j) == ref[i+3*j]; }, view.shape())) );
}

TTS_CASE( "Build a 2D view with constexpr shape settings from a ContiguousSequence" )
{
  std::vector<float> ref = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

  kwk::view<float,kwk::extent[3][5]> view(ref);

  TTS_EQUAL( sizeof(view), sizeof(void*)      );
  TTS_EQUAL( view.size() , 15ULL              );
  TTS_EQUAL( view.count(), 15                 );
  TTS_EQUAL( view.shape(), kwk::of_shape(3,5) );

  TTS_EXPECT( (std::equal( view.begin(), view.end(), &ref[0])) );
  TTS_EXPECT( (kwk::all( [&](auto i, auto j) { return view(i,j) == ref[i+3*j]; }, view.shape())) );
}

TTS_CASE( "Build a 2D view with constexpr shape settings from a const ContiguousSequence" )
{
  std::vector<float> const ref = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

  kwk::view<float const,kwk::extent[3][5]> view(ref);

  TTS_EQUAL( sizeof(view), sizeof(void*)      );
  TTS_EQUAL( view.size() , 15ULL              );
  TTS_EQUAL( view.count(), 15                 );
  TTS_EQUAL( view.shape(), kwk::of_shape(3,5) );

  TTS_EXPECT( (std::equal( view.begin(), view.end(), &ref[0])) );
  TTS_EXPECT( (kwk::all( [&](auto i, auto j) { return view(i,j) == ref[i+3*j]; }, view.shape())) );
}
