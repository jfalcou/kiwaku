//======================================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//======================================================================================================================
#pragma once

#include <kwk/concepts/container.hpp>
#include <kwk/context/context.hpp>
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/predicates.hpp>
#include <kwk/detail/algorithm/for_until.hpp>
#include <kwk/algorithm/views/reverse.hpp>
#include <kwk/detail/abi.hpp>
#include <kwk/utility/position.hpp>
#include <cstddef>
#include <utility>
#include <optional>

#include <kwk/detail/sequence/prefilled.hpp>
// ALGO:
/*
adjacent_find -> PB ND
search -> attendre tuilage
find_end -> search_last
*/

// For_each interruptible + renvoi optional
namespace kwk
{

  // Unused context
  template<typename Context, concepts::container Container, typename Check>
  std::optional<kwk::position<Container::static_order>>
  find_if(Context&, Container const& c, Check f)
  {
    // TODO: if no element found, return a kumi tuple containing the size of each dimension
    // and NOT -1 for each dimension.
    kwk::position<Container::static_order> pos;

    // TODO: replace kwk::__::for_until with kwk::for_until?
    bool valid_position = kwk::__::for_until( [&](auto... is)
                        {
                          if(f(c(is...)))
                          {
                            unsigned long poss[Container::static_order] = {is...};
                            for (std::size_t i = 0; i < Container::static_order; ++i)
                            {
                              pos[i] = static_cast<typename kwk::position<Container::static_order>::position_type>(poss[i]);
                            }
                            return true;
                          }
                          return false;
                        }
                      , c.shape()
                      );
      
    if (valid_position) return std::optional<kwk::position<Container::static_order>>{pos};
    else                return std::nullopt;

  }

  template<concepts::container Container, typename Check>
  std::optional<kwk::position<Container::static_order>>
  find_if(Container const& c, Check f)
  {
    return kwk::find_if(cpu, c, f);
  }

  template <typename Context, concepts::container Out>
  constexpr std::optional<kwk::position<Out::static_order>>
  find(Context& ctx, Out const& out, auto value)
  {
    // Only uses value inside the kernel
    return find_if(ctx, out, [value](auto e){return e == value;});
  }

  template <concepts::container Out>
  constexpr std::optional<kwk::position<Out::static_order>>
  find(Out const& out, auto value)
  {
    return find(cpu, out, value);
  }

  template <typename Context, typename Func, concepts::container Out>
  constexpr std::optional<kwk::position<Out::static_order>>
  find_if_not(Context& ctx, Out const& out, Func f)
  {
    return find_if(ctx, out, [f](auto x){return !f(x);});
  }

  template <typename Func, concepts::container Out>
  constexpr std::optional<kwk::position<Out::static_order>>
  find_if_not(Out const& out, Func f)
  {
    return kwk::find_if_not(cpu, out, f);
  }

  template <typename Context, typename Func, concepts::container Out>
  constexpr std::optional<kwk::position<Out::static_order>>
  find_last_if(Context& ctx, Out const& out, Func f)
  {
    auto res = find_if(ctx, kwk::reverse(out), f);

    if (res.has_value())
    {
      // Reverse positions
      kumi::for_each_index( [&](auto i, auto) { res.value()[i] = kwk::get<i>(out.shape()) - 1 - res.value()[i]; }
                          , out.shape()
                          );
    }

    return res;
  }

  template <typename Func, concepts::container Out>
  constexpr std::optional<kwk::position<Out::static_order>>
  find_last_if(Out const& out, Func f)
  {
    return find_last_if(cpu, out, f);
  }

  template <typename Context, concepts::container Out>
  constexpr std::optional<kwk::position<Out::static_order>>
  find_last(Context& ctx, Out const& out, auto value)
  {
    return find_last_if(ctx, out, [&](auto e){return e == value;});
  }

  template <concepts::container Out>
  constexpr std::optional<kwk::position<Out::static_order>>
  find_last(Out const& out, auto value)
  {
    return kwk::find_last(cpu, out, value);
  }

  template <typename Context, typename Func, concepts::container Out>
  constexpr std::optional<kwk::position<Out::static_order>>
  find_last_if_not(Context& ctx, Out const& out, Func f)
  {
    return find_last_if(ctx, out, [f](auto x){return !f(x);});
  }

  template <typename Func, concepts::container Out>
  constexpr std::optional<kwk::position<Out::static_order>>
  find_last_if_not(Out const& out, Func f)
  {
    return kwk::find_last_if_not(cpu, out, f);
  }
}
