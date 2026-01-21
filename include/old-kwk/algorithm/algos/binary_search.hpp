//======================================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <kwk/container.hpp>
#include <kwk/context/context.hpp>
#include <kwk/utility/coordinates.hpp>
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/detail/abi.hpp>
#include <cstddef>
#include <utility>
#include <optional>


// trouver un moyen malin de calculer la position
// échanger de place func et non func et appeler func
namespace kwk
{

  // Unused context
  // Function overloaded, even if Context is not used: for overloading with other contexts.
  // eg.:
  // constexpr auto lower_bound(sycl::context& c, Out const& out, auto value, Func func)
  template <typename Context, typename Func, concepts::container Out>
  constexpr std::optional<std::array<int,Out::static_order>>
  lower_bound(Context&, Out const& out, auto value, Func func)
  {
    if (out.numel() == 0) return std::nullopt;

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
    return (first < out.numel())  ?  std::optional<std::array<int,Out::static_order>>{kwk::coordinates(it, out.shape())} 
                                  :  std::nullopt;
  }
  template <typename Func, concepts::container Out>
  constexpr auto lower_bound(Out const& out, auto value, Func func)
  {
    return lower_bound(cpu, out, value, func); // Unused context
  }

  template <typename Context, concepts::container Out>
  constexpr auto lower_bound(Context& ctx, Out const& out, auto value)
  {
    return lower_bound(ctx, out, value, [](auto e, auto i){return e<i;});
  }
  template <concepts::container Out>
  constexpr auto lower_bound(Out const& out, auto value)
  {
    return lower_bound(cpu, out, value);
  }

  // Unused context
  template <typename Context, typename Func, concepts::container Out>
  constexpr auto upper_bound(Context&, Out const& out, auto value, Func func)
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
    return (first < out.numel())  ?  std::optional<std::array<int,Out::static_order>>{kwk::coordinates(it, out.shape())} 
                                  :  std::nullopt;
  }
  template <typename Func, concepts::container Out>
  constexpr auto upper_bound(Out const& out, auto value, Func func)
  {
    return upper_bound(cpu, out, value, func);
  }


  template <typename Context, concepts::container Out>
  constexpr auto upper_bound(Context& ctx, Out const& out, auto value)
  {
    return upper_bound(ctx, out, value, [](auto e, auto i){return e<i;});
  }
  template <concepts::container Out>
  constexpr auto upper_bound(Out const& out, auto value)
  {
    return upper_bound(cpu, out, value);
  }


  template <typename Context, typename Func, concepts::container Out>
  constexpr bool binary_search(Context& ctx, Out const& out, auto value, Func func)
  {
    // Finds the index of the first element before (or equal to) value
    //    returns an std::array<int, dimensions>
    auto first = lower_bound(ctx, out, value, func);

    // Element not found
    if (first == std::nullopt) return false;

    // transforms the array into a kumi::tuple
    auto kumi_findex = std::apply([](auto... i) { return kumi::tuple{i...}; }, *first);

    // return (out(kumi_findex) == value);
    return !(func(value, out(kumi_findex)));

  }
  template <typename Func, concepts::container Out>
  constexpr bool binary_search(Out const& out, auto value, Func func)
  {
    return binary_search(cpu, out, value, func);
  }

  template <typename Context, concepts::container Out>
  constexpr bool binary_search(Context& ctx, Out const& out, auto value)
  {
    return binary_search(ctx, out, value, [](auto e, auto i){return e<i;});
  }
  template <concepts::container Out>
  constexpr bool binary_search(Out const& out, auto value)
  {
    return binary_search(cpu, out, value);
  }
}
