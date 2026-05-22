//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kumi/kumi.hpp>

namespace kwk::__
{
  template<typename... Coords>
  constexpr auto setup_linear_interpolation(Coords... p) noexcept
  {
    return kumi::fold_left
    ( [](auto acc, auto v)
      {
        // Extarct integral and decimal parts of current coordinate
        auto mp = static_cast<int>(v);

        // Find the discrete boundaries
        auto b  = v-mp;
        auto a  = 1-b;

        auto[coord,coeff] = acc;

        // Update interpolation coefficients
        auto cf0 = kumi::map( [a](auto e) { return e*a; }, coeff);
        auto cf1 = kumi::map( [b](auto e) { return e*b; }, coeff);

        // Update nD discrete coordinates
        auto cmp = kumi::map( [&](auto e) { return push_back(e,mp);   }, coord);
        auto cnp = kumi::map( [&](auto e) { return push_back(e,mp+1); }, coord);

        return kumi::make_tuple( kumi::cat(cmp,cnp), kumi::cat(cf0,cf1));
      }
    , kumi::make_tuple(p...)
    , kumi::make_tuple( kumi::make_tuple(kumi::tuple{}),kumi::tuple{1.f})
    );
  }

  struct linear_
  {
    template<typename... Coords>
    auto interpolate(auto const& data, Coords... p) const
    {
      // setup_linear_interpolation computes the 2^N discrete coordinates and associated
      // coefficients
      auto[coord,coeff] = __::setup_linear_interpolation(p...);

      // Turn the discrete indexing into values
      auto pos = kumi::map( [&](auto v){ return data(v); }, coord );

      // A n-linear interpolation is then computed as the inner product of the values at
      // the 2^n discrete position and the compound coefficient derived from the
      // non-integral parts of the requested point.
      return kumi::inner_product(pop_front(pos),pop_front(coeff),front(pos)*front(coeff));
    }
  };

  struct floor_
  {
    template<typename Container, typename... Coords>
    auto interpolate(Container const& data, Coords... p) const
    {
      return data(static_cast<std::ptrdiff_t>(p)...);
    }
  };
}

namespace kwk
{
  //================================================================================================
  //! @brief Round-to-floor interpolation strategy
  //================================================================================================
  inline constexpr __::floor_ floor = {};

  //================================================================================================
  //! @brief Linear interpolation strategy
  //================================================================================================
  inline constexpr __::linear_ linear = {};
}
