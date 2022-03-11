//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/container/stride.hpp>
#include <algorithm>
#include <numeric>
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
};

TTS_CASE( "2D Index computation" )
{
  kwk::stride st{2,6};

  std::vector<std::ptrdiff_t> ref;
  std::vector<std::ptrdiff_t> output;

  {
    for(auto i=0;i<15;++i)
      ref.push_back(2*i);

    for(auto i=0;i<15;++i)
      output.push_back( st.index(i) );

    TTS_ALL_EQUAL(output, ref);
  }

  {
    for(auto j=0;j<5;++j)
      for(auto i=0;i<3;++i)
      ref.push_back(2*i + 6*j);

    for(auto j=0;j<5;++j)
      for(auto i=0;i<3;++i)
      output.push_back( st.index(i,j) );

    TTS_ALL_EQUAL(output, ref);
  }
};

TTS_CASE( "3D index computation" )
{
  kwk::stride st{2,6,30};

  std::vector<std::ptrdiff_t> ref;
  std::vector<std::ptrdiff_t> output;

  {
    for(auto i=0;i<60;++i)
      ref.push_back(2*i);

    for(auto i=0;i<60;++i)
      output.push_back( st.index(i) );

    TTS_ALL_EQUAL(output, ref);
  }

  {
    for(auto j=0;j<20;++j)
      for(auto i=0;i<3;++i)
      ref.push_back(2*i + 6*j);

    for(auto j=0;j<20;++j)
      for(auto i=0;i<3;++i)
      output.push_back( st.index(i,j) );

    TTS_ALL_EQUAL(output, ref);
  }

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
};

TTS_CASE( "4D index computation" )
{
  kwk::stride st{2,6,30,60};

  std::vector<std::ptrdiff_t> ref;
  std::vector<std::ptrdiff_t> output;

  {
    for(auto i=0;i<60;++i)
      ref.push_back(2*i);

    for(auto i=0;i<60;++i)
      output.push_back( st.index(i) );

    TTS_ALL_EQUAL(output, ref);
  }

  {
    for(auto j=0;j<20;++j)
      for(auto i=0;i<3;++i)
      ref.push_back(2*i + 6*j);

    for(auto j=0;j<20;++j)
      for(auto i=0;i<3;++i)
      output.push_back( st.index(i,j) );

    TTS_ALL_EQUAL(output, ref);
  }

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
};
