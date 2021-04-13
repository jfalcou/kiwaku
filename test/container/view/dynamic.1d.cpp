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
#include <tts/tests/relation.hpp>
#include <tts/tests/basic.hpp>
#include <algorithm>
#include <vector>

TTS_CASE( "Build a 1D view with runtime shape settings from a C array" )
{
  float ref[7] = {1,2,3,4,5,6,7};

  kwk::view view(ref,kwk::of_shape(7));

  TTS_EQUAL( sizeof(view), sizeof(void*)*2  );
  TTS_EQUAL( view.size() , 7ULL             );
  TTS_EQUAL( view.count(), 7                );
  TTS_EQUAL( view.shape(), kwk::of_shape(7) );

  TTS_EXPECT( (std::equal( view.begin(), view.end(), &ref[0])) );
  TTS_EXPECT( (kwk::all ( [&](auto i) { return view(i) == ref[i]; }, view.shape())) );
}

TTS_CASE( "Build a 1D view with runtime shape settings from a pointer" )
{
  float ref[] = {1,2,3,4,5,6,7};

  kwk::view view(&ref[0],kwk::of_shape(7));

  TTS_EQUAL( sizeof(view), sizeof(void*)*2  );
  TTS_EQUAL( view.size() , 7ULL             );
  TTS_EQUAL( view.count(), 7                );
  TTS_EQUAL( view.shape(), kwk::of_shape(7) );

  TTS_EXPECT( (std::equal( view.begin(), view.end(), &ref[0])) );
  TTS_EXPECT( (kwk::all ( [&](auto i) { return view(i) == ref[i]; }, view.shape())) );
}

TTS_CASE( "Build a 1D view with runtime shape settings from a pointer to const" )
{
  float const ref[] = {1,2,3,4,5,6,7};

  kwk::view<float const,kwk::_1D> view(&ref[0],kwk::of_shape(7));

  TTS_EQUAL( sizeof(view), sizeof(void*)*2  );
  TTS_EQUAL( view.size() , 7ULL             );
  TTS_EQUAL( view.count(), 7                );
  TTS_EQUAL( view.shape(), kwk::of_shape(7) );

  TTS_EXPECT( (std::equal( view.begin(), view.end(), &ref[0])) );
  TTS_EXPECT( (kwk::all ( [&](auto i) { return view(i) == ref[i]; }, view.shape())) );
}

TTS_CASE( "Build a 1D view with runtime shape settings from a ContiguousSequence" )
{
  std::vector<float> ref = {1,2,3,4,5,6,7};

  kwk::view view(ref,kwk::of_shape(7));

  TTS_EQUAL( sizeof(view), sizeof(void*)*2  );
  TTS_EQUAL( view.size() , 7ULL             );
  TTS_EQUAL( view.count(), 7                );
  TTS_EQUAL( view.shape(), kwk::of_shape(7) );

  TTS_EXPECT( (std::equal( view.begin(), view.end(), &ref[0])) );
  TTS_EXPECT( (kwk::all ( [&](auto i) { return view(i) == ref[i]; }, view.shape())) );
}

TTS_CASE( "Build a 1D view with runtime shape settings from a const ContiguousSequence" )
{
  std::vector<float> const ref = {1,2,3,4,5,6,7};

  kwk::view<float const,kwk::_1D> view(ref,kwk::of_shape(7));

  TTS_EQUAL( sizeof(view), sizeof(void*)*2  );
  TTS_EQUAL( view.size() , 7ULL             );
  TTS_EQUAL( view.count(), 7                );
  TTS_EQUAL( view.shape(), kwk::of_shape(7) );

  TTS_EXPECT( (std::equal( view.begin(), view.end(), &ref[0])) );
  TTS_EXPECT( (kwk::all ( [&](auto i) { return view(i) == ref[i]; }, view.shape())) );
}
