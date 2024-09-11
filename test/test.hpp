//======================================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once
#include <type_traits>
#include <utility>
#include <kwk/detail/abi.hpp>
#include <kwk/utility/linear_index.hpp>
#include <cmath>

template<int N> using int_ = std::integral_constant<int,N>;
template<int N> struct nD : std::integral_constant<int,N> {};
template<typename N> using up_to = std::make_index_sequence<N::value>;

#define TTS_MAIN
#define TTS_CUSTOM_DRIVER_FUNCTION kwk_entry_point
#include "tts.hpp"

#define FLOAT_TOLERANCE_PERCENT 0.01


kumi::tuple<int, int, int, int> lindex_to_pos ( std::size_t const d1
                                                    , std::size_t const d2
                                                    , std::size_t const d3
                                                    , int const lindex
                                                    )
{
  int remains = lindex;
  std::array<int, 4> p;
  std::fill(p.begin(), p.end(), 0);
  p[0] = remains / (d1 * d2 * d3);
  remains -= p[0] * (d1 * d2 * d3);
  p[1] = remains / (d2 * d3);
  remains -= p[1] * (d2 * d3);
  p[2] = remains / d3;
  remains -= p[2] * d3;
  p[3] = remains;

  return kumi::tuple<int, int, int, int>{p[0], p[1], p[2], p[3]};
}

kumi::tuple<int, int> lindex_to_pos ( std::size_t const d1
                                    , int const lindex
                                    )
{
  int remains = lindex;
  std::array<int, 2> p;
  std::fill(p.begin(), p.end(), 0);
  p[0] = remains / (d1);
  remains -= p[0] * (d1);
  p[1] = remains;

  return kumi::tuple<int, int>{p[0], p[1]};
}

std::array<std::size_t, 4> lindex_to_pos_arr( std::size_t const d1
                                    , std::size_t const d2
                                    , std::size_t const d3
                                    , int const lindex
                                    )
{
  int remains = lindex;
  std::array<std::size_t, 4> p;
  std::fill(p.begin(), p.end(), 0);
  p[0] = remains / (d1 * d2 * d3);
  remains -= p[0] * (d1 * d2 * d3);
  p[1] = remains / (d2 * d3);
  remains -= p[1] * (d2 * d3);
  p[2] = remains / d3;
  remains -= p[2] * d3;
  p[3] = remains;

  return p;
}

std::array<std::size_t, 2> lindex_to_pos_arr( std::size_t const d1
                                    , int const lindex
                                    )
{
  int remains = lindex;
  std::array<std::size_t, 2> p;
  std::fill(p.begin(), p.end(), 0);
  p[0] = remains / d1;
  remains -= p[0] * d1;
  p[1] = remains;

  return p;
}

std::array<std::size_t, 4> std_res_to_pos ( int* res
                                          , int* begin
                                          , std::size_t const d1
                                          , std::size_t const d2
                                          , std::size_t const d3
                                          )
{
  std::size_t dist = static_cast<std::size_t>(std::distance(begin, res));
  return lindex_to_pos_arr(d1, d2, d3, dist);
}

std::array<std::size_t, 2> std_res_to_pos ( int* res
                                          , int* begin
                                          , std::size_t const d1
                                          )
{
  std::size_t dist = static_cast<std::size_t>(std::distance(begin, res));
  return lindex_to_pos_arr(d1, dist);
}



template<typename T, typename S>
void fill_data(T* data, S shp, bool fill)
requires(S::static_order == 1)
{
  std::cout << get<0>(shp) << "\n";
  for(int i1 = 0; i1 < get<0>(shp); i1++)
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
