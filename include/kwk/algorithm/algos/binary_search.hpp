//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/container.hpp>
#include <kwk/utility/coordinates.hpp>
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/detail/abi.hpp>
#include <cstddef>
#include <utility>
#include <optional>

// ALGO:
/*
equal_range // view entre les deux boundss  
*/

// trouver un moyen malin de calculer la position
// échanger de place func et non func et appeler func
namespace kwk
{
  template <typename Func, concepts::container Out>
  constexpr auto lower_bound(Out const& out, auto value, Func func)
  {
    auto count = out.numel();
    auto first = 0;
    auto it = 0; 
    auto step = 0;

    while (count > 0)
    {
        it = first;
        step = count / 2; 
        it += step;

        auto p = kwk::coordinates(it, out.shape());
        auto pos = std::apply([](auto... i) { return kumi::tuple{i...}; }, p);
        if (func(out(pos), value))
        {
            first = ++it; 
            count -= step + 1; 
        }
        else
            count = step;
    }
    return (first < out.numel()) ?  std::optional<std::array<int,Out::static_order>>{kwk::coordinates(it, out.shape())} 
                                  :  std::nullopt;
  }

  template <concepts::container Out>
  constexpr auto lower_bound(Out const& out, auto value)
  {
    return lower_bound(out, value, [](auto e, auto i){return e<i;});
  }

  template <typename Func, concepts::container Out>
  constexpr auto upper_bound(Out const& out, auto value, Func func)
  {
    auto count = out.numel();
    auto first = 0;
    auto it = 0; 
    auto step = 0;

    while (count > 0)
    {
        it = first; 
        step = count / 2; 
        it += step;
 
        auto p = kwk::coordinates(it, out.shape());
        auto pos = std::apply([](auto... i) { return kumi::tuple{i...}; }, p);
        if (!func(value, out(pos)))
        {
            first = ++it; 
            count -= step + 1; 
        }
        else
            count = step;
    }
    return (first < out.numel()) ?  std::optional<std::array<int,Out::static_order>>{kwk::coordinates(it, out.shape())} 
                                  :  std::nullopt;
  }

  template <concepts::container Out>
  constexpr auto upper_bound(Out const& out, auto value)
  {
    return upper_bound( out, value, [](auto e, auto i){return e<i;});
  }

  template <typename Func, concepts::container Out>
  constexpr bool binary_search(Out const& out, auto value, Func func)
  {
    auto first = kwk::coordinates(0, out.shape());
    auto f = std::apply([](auto... i) { return kumi::tuple{i...}; }, first);

    if (func(value, out(f))) return false;

    auto p = kwk::lower_bound(out, value, func);
    bool outbound;

    if(p)
      outbound = false;
    else
      outbound = true;

    return (!outbound);
  }

  template <concepts::container Out>
  constexpr bool binary_search(Out const& out, auto value)
  {
    return binary_search(out, value, [](auto e, auto i){return e<i;});
  }
}
