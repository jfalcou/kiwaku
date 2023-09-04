//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once
#include <type_traits>
#include <utility>
#include <kwk/detail/abi.hpp>
#include <kwk/utility/linear_index.hpp>

template<int N> using int_ = std::integral_constant<int,N>;
template<int N> struct nD : std::integral_constant<int,N> {};
template<typename N> using up_to = std::make_index_sequence<N::value>;

#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION kwk_entry_point
#include "tts.hpp"

template<typename T, typename S>
void fill_data(T* data, S shp, bool fill)
requires(S::static_order == 1)
{
  for(int i1 = 0; i1<get<0>(shp); i1++)
  {
    if(!fill) data[i1] = 0;
    else data[i1] = T(i1);
  }
}

template<typename T, typename S>
void fill_data(T* data, S shp, bool fill)
requires(S::static_order == 2)
{
  for(int i1 = 0; i1<get<0>(shp); i1++)
  {
    for(int i2 = 0; i2<get<1>(shp); i2++)
    {
      int index = kwk::linear_index(shp, i1, i2);
      if(!fill) data[index] = 0;
      else data[index] = T(i1 * 10 + i2);
    }
  }
}

template<typename T, typename S>
void fill_data(T* data, S shp, bool fill)
requires(S::static_order == 3)
{
  for(int i1 = 0; i1<get<0>(shp); i1++)
  {
    for(int i2 = 0; i2<get<1>(shp); i2++)
    {
      for(int i3 = 0; i3<get<2>(shp); i3++)
      {
        int index = kwk::linear_index(shp, i1, i2, i3);
        if(!fill) data[index] = 0;
        else data[index] = T(i1 * 100 + i2 * 10 + i3);
      }
    }
  }
}

template<typename T, typename S>
void fill_data(T* data, S shp, bool fill)
requires(S::static_order == 4)
{
  for(int i1 = 0; i1<get<0>(shp); i1++)
  {
    for(int i2 = 0; i2<get<1>(shp); i2++)
    {
      for(int i3 = 0; i3<get<2>(shp); i3++)
      {
        for(int i4 = 0; i4<get<3>(shp); i4++)
        {
          int index = kwk::linear_index(shp, i1, i2, i3, i4);
          if(!fill) data[index] = 0;
          else data[index] = T(i1 * 1000 + i2 * 100 + i3 * 10 + i4);
        }
      }
    }
  }
}

int main(int argc, char const **argv)
{
  std::cout << "[Kiwaku] - Assertions: ";
#ifdef NDEBUG
  std::cout << "Disabled\n";
#else
  std::cout << "Enabled\n";
#endif

  kwk_entry_point(argc, argv);
  return tts::report(0,0);
}
