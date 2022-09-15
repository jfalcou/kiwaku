//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier:   KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#include "test.hpp"
#include <kwk/container/stride.hpp>
#include <kwk/container/shape.hpp>
#include <algorithm>
#include <numeric>
#include <vector>

TTS_CASE( "1D index computation from unit stride" )
{
  kwk::shape        sh{13};
  auto st = sh.as_stride();

  std::vector<std::ptrdiff_t> ref(13);
  std::iota(ref.begin(),ref.end(), 0);

  std::vector<std::ptrdiff_t> output(13);
  std::transform(ref.begin(),ref.end(), output.begin(), [&](auto e) { return st.index(e); } );

  TTS_ALL_EQUAL(output, ref);
};

TTS_CASE( "2D Index computation from unit stride" )
{
  kwk::shape       sh{6,7};
  auto st = sh.as_stride();

  std::vector<std::ptrdiff_t> ref(sh.numel());
  std::iota(ref.begin(),ref.end(), 0);

  {
    std::vector<std::ptrdiff_t> output;
    for(auto i=0;i<sh.numel();i++)
      output.push_back( st.index(i) );

    TTS_ALL_EQUAL(output, ref);
  }

  {
    std::vector<std::ptrdiff_t> output;
    auto[sh0,sh1] = sh;
    for(auto j=0;j<sh1;j++)
      for(auto i=0;i<sh0;i++)
      output.push_back( st.index(i,j) );

    TTS_ALL_EQUAL(output, ref);
  }
};

TTS_CASE( "3D index computation from unit stride" )
{
  kwk::shape        sh{2,3,5};
  auto st = sh.as_stride();

  std::vector<std::ptrdiff_t> ref(sh.numel());
  std::iota(ref.begin(),ref.end(), 0);

  {
    std::vector<std::ptrdiff_t> output;
    for(auto i=0;i<sh.numel();i++)
      output.push_back( st.index(i) );

    TTS_ALL_EQUAL(output, ref);
  }

  {
    auto[sh0,sh1,sh2] = sh;
    std::vector<std::ptrdiff_t> output;
    for(auto j=0;j<sh1*sh2;j++)
      for(auto i=0;i<sh0;i++)
      output.push_back( st.index(i,j) );

    TTS_ALL_EQUAL(output, ref);
  }

  {
    auto[sh0,sh1,sh2] = sh;
    std::vector<std::ptrdiff_t> output;
    for(auto k=0;k<sh2;k++)
      for(auto j=0;j<sh1;j++)
      for(auto i=0;i<sh0;i++)
      output.push_back( st.index(i,j,k) );

    TTS_ALL_EQUAL(output, ref);
  }
};

TTS_CASE( "4D index computation from unit stride" )
{
  kwk::shape        sh{2,3,5,7};
  auto st = sh.as_stride();

  std::vector<std::ptrdiff_t> ref(sh.numel());
  std::iota(ref.begin(),ref.end(), 0);

  {
    std::vector<std::ptrdiff_t> output;

    for(auto i=0;i<sh.numel();i++)
      output.push_back( st.index(i) );

    TTS_ALL_EQUAL(output, ref);
  }

  {
    std::vector<std::ptrdiff_t> output;
    auto[sh0,sh1,sh2,sh3] = sh;
    for(auto j=0;j<sh1*sh2*sh3;j++)
      for(auto i=0;i<sh0;i++)
      output.push_back( st.index(i,j) );

    TTS_ALL_EQUAL(output, ref);
  }

  {
    std::vector<std::ptrdiff_t> output;
    auto[sh0,sh1,sh2,sh3] = sh;
    for(auto k=0;k<sh2*sh3;k++)
      for(auto j=0;j<sh1;j++)
      for(auto i=0;i<sh0;i++)
      output.push_back( st.index(i,j,k) );

    TTS_ALL_EQUAL(output, ref);
  }

  {
    std::vector<std::ptrdiff_t> output;
    auto[sh0,sh1,sh2,sh3] = sh;
    for(auto l=0;l<sh3;l++)
      for(auto k=0;k<sh2;k++)
      for(auto j=0;j<sh1;j++)
        for(auto i=0;i<sh0;i++)
        output.push_back( st.index(i,j,k,l) );

    TTS_ALL_EQUAL(output, ref);
  }
};
