//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/misc/stride.hpp>
#include <tts/tests/relation.hpp>
#include <tts/tests/sequence.hpp>
#include <algorithm>
#include <vector>

TTS_CASE( "1D index computation" )
{
  kwk::stride  st{2};

  std::vector<std::ptrdiff_t> base(13);
  std::vector<std::ptrdiff_t> ref(13);
  std::vector<std::ptrdiff_t> output(13);

  std::iota(base.begin(),base.end(), 0);

  for(auto i=0;i<13;++i) ref[i] = 2*base[i];
  std::transform(base.begin(),base.end(), output.begin(), [&](auto e) { return st.index(e); } );

  TTS_ALL_EQUAL(output, ref);
}

TTS_CASE( "2D Index computation" )
{
  kwk::stride st{2,6};

  TTS_WHEN("we build a 2D stride")
  {
    std::vector<std::ptrdiff_t> ref;
    std::vector<std::ptrdiff_t> output;

    TTS_AND_THEN( "we compute linear index from 1D positions" )
    {
      for(auto i=0;i<15;++i)
        ref.push_back(2*i);

      for(auto i=0;i<15;++i)
        output.push_back( st.index(i) );

      TTS_ALL_EQUAL(output, ref);
    }

    TTS_AND_THEN( "we compute linear index from 2D positions" )
    {
      for(auto j=0;j<5;++j)
       for(auto i=0;i<3;++i)
        ref.push_back(2*i + 6*j);

      for(auto j=0;j<5;++j)
       for(auto i=0;i<3;++i)
        output.push_back( st.index(i,j) );

      TTS_ALL_EQUAL(output, ref);
    }
  }
}

TTS_CASE( "3D index computation" )
{
  kwk::stride st{2,6,30};

  TTS_WHEN("we build a 3D stride")
  {
    std::vector<std::ptrdiff_t> ref;
    std::vector<std::ptrdiff_t> output;
    TTS_AND_THEN( "we compute linear index from 1D positions" )
    {
      for(auto i=0;i<60;++i)
        ref.push_back(2*i);

      for(auto i=0;i<60;++i)
        output.push_back( st.index(i) );

      TTS_ALL_EQUAL(output, ref);
    }

    TTS_AND_THEN( "we compute linear index from 2D positions" )
    {
      for(auto j=0;j<20;++j)
       for(auto i=0;i<3;++i)
        ref.push_back(2*i + 6*j);

      for(auto j=0;j<20;++j)
       for(auto i=0;i<3;++i)
        output.push_back( st.index(i,j) );

      TTS_ALL_EQUAL(output, ref);
    }

    TTS_AND_THEN( "we compute linear index from 3D positions" )
    {
      for(auto k=0;k<4;++k)
       for(auto j=0;j<5;++j)
        for(auto i=0;i<3;++i)
        ref.push_back(2*i + 6*j + 30*k);

      for(auto k=0;k<4;++k)
       for(auto j=0;j<5;++j)
        for(auto i=0;i<3;++i)
        output.push_back( st.index(i,j,k) );

      TTS_ALL_EQUAL(output, ref);
    }
  }
}

TTS_CASE( "4D index computation" )
{
  kwk::stride st{2,6,30,60};

  TTS_WHEN("we build a 4D stride")
  {
    std::vector<std::ptrdiff_t> ref;
    std::vector<std::ptrdiff_t> output;
    TTS_AND_THEN( "we compute linear index from 1D positions" )
    {
      for(auto i=0;i<60;++i)
        ref.push_back(2*i);

      for(auto i=0;i<60;++i)
        output.push_back( st.index(i) );

      TTS_ALL_EQUAL(output, ref);
    }

    TTS_AND_THEN( "we compute linear index from 2D positions" )
    {
      for(auto j=0;j<20;++j)
       for(auto i=0;i<3;++i)
        ref.push_back(2*i + 6*j);

      for(auto j=0;j<20;++j)
       for(auto i=0;i<3;++i)
        output.push_back( st.index(i,j) );

      TTS_ALL_EQUAL(output, ref);
    }

    TTS_AND_THEN( "we compute linear index from 3D positions" )
    {
      for(auto k=0;k<4;++k)
       for(auto j=0;j<5;++j)
        for(auto i=0;i<3;++i)
        ref.push_back(2*i + 6*j + 30*k);

      for(auto k=0;k<4;++k)
       for(auto j=0;j<5;++j)
        for(auto i=0;i<3;++i)
        output.push_back( st.index(i,j,k) );

      TTS_ALL_EQUAL(output, ref);
    }

    TTS_AND_THEN( "we compute linear index from 4D positions" )
    {
      for(auto l=0;l<2;++l)
       for(auto k=0;k<4;++k)
        for(auto j=0;j<5;++j)
         for(auto i=0;i<3;++i)
          ref.push_back(2*i + 6*j + 30*k + 60*l);

      for(auto l=0;l<2;++l)
       for(auto k=0;k<4;++k)
        for(auto j=0;j<5;++j)
         for(auto i=0;i<3;++i)
          output.push_back( st.index(i,j,k,l) );

      TTS_ALL_EQUAL(output, ref);
    }
  }
}
