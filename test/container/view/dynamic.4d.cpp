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

TTS_CASE( "Build a 4D view with constexpr shape settings from a C array" )
{
  float ref[24] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};

  kwk::view<float,kwk::_4D> view(ref,kwk::of_shape(4,1,3,2));

  TTS_EQUAL( sizeof(view), sizeof(void*)*4*2      );
  TTS_EQUAL( view.size() , 24                     );
  TTS_EQUAL( view.shape(), kwk::of_shape(4,1,3,2) );

  TTS_EXPECT( (std::equal( view.begin(), view.end(), &ref[0])) );
  TTS_EXPECT( (kwk::all ( [&](auto i, auto j, auto k, auto l)
                          {
                            return view(i,j,k,l) == ref[i+4*(j+1*(k+3*l))];
                          }
                        , view.shape())
                        )
            );
}

TTS_CASE( "Build a 4D view with constexpr shape settings from a pointer" )
{
  float ref[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};

  kwk::view<float,kwk::_4D> view(&ref[0],kwk::of_shape(4,1,3,2));

  TTS_EQUAL( sizeof(view), sizeof(void*)*4*2      );
  TTS_EQUAL( view.size() , 24                     );
  TTS_EQUAL( view.shape(), kwk::of_shape(4,1,3,2) );

  TTS_EXPECT( (std::equal( view.begin(), view.end(), &ref[0])) );
  TTS_EXPECT( (kwk::all ( [&](auto i, auto j, auto k, auto l)
                          {
                            return view(i,j,k,l) == ref[i+4*(j+1*(k+3*l))];
                          }
                        , view.shape())
                        )
            );
}

TTS_CASE( "Build a 4D view with constexpr shape settings from a pointer to const" )
{
  float const ref[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};

  kwk::view<float const,kwk::_4D> view(&ref[0],kwk::of_shape(4,1,3,2));

  TTS_EQUAL( sizeof(view), sizeof(void*)*4*2      );
  TTS_EQUAL( view.size() , 24                     );
  TTS_EQUAL( view.shape(), kwk::of_shape(4,1,3,2) );

  TTS_EXPECT( (std::equal( view.begin(), view.end(), &ref[0])) );
  TTS_EXPECT( (kwk::all ( [&](auto i, auto j, auto k, auto l)
                          {
                            return view(i,j,k,l) == ref[i+4*(j+1*(k+3*l))];
                          }
                        , view.shape())
                        )
            );
}

TTS_CASE( "Build a 4D view with constexpr shape settings from a ContiguousSequence" )
{
  std::vector<float> ref = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};

  kwk::view<float,kwk::_4D> view(ref,kwk::of_shape(4,1,3,2));

  TTS_EQUAL( sizeof(view), sizeof(void*)*4*2      );
  TTS_EQUAL( view.size() , 24                     );
  TTS_EQUAL( view.shape(), kwk::of_shape(4,1,3,2) );

  TTS_EXPECT( (std::equal( view.begin(), view.end(), &ref[0])) );
  TTS_EXPECT( (kwk::all ( [&](auto i, auto j, auto k, auto l)
                          {
                            return view(i,j,k,l) == ref[i+4*(j+1*(k+3*l))];
                          }
                        , view.shape())
                        )
            );
}

TTS_CASE( "Build a 4D view with constexpr shape settings from a const ContiguousSequence" )
{
  std::vector<float> const ref = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};

  kwk::view<float const,kwk::_4D> view(ref,kwk::of_shape(4,1,3,2));

  TTS_EQUAL( sizeof(view), sizeof(void*)*4*2      );
  TTS_EQUAL( view.size() , 24                     );
  TTS_EQUAL( view.shape(), kwk::of_shape(4,1,3,2) );

  TTS_EXPECT( (std::equal( view.begin(), view.end(), &ref[0])) );
  TTS_EXPECT( (kwk::all ( [&](auto i, auto j, auto k, auto l)
                          {
                            return view(i,j,k,l) == ref[i+4*(j+1*(k+3*l))];
                          }
                        , view.shape())
                        )
            );
}
