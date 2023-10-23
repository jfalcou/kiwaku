//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/concepts/container.hpp>
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/predicates.hpp>
#include <kwk/detail/algorithm/for_until.hpp>
#include <kwk/algorithm/views/reverse.hpp>
#include <kwk/detail/abi.hpp>
#include <cstddef>
#include <utility>

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
  template<typename Context, typename Container, typename Check>
  auto find_if(Context&, Container const& c, Check f)
  {
    auto pos = kumi::generate<Container::static_order>(-1);

    // TODO: replace kwk::__::for_until with kwk::for_until?
    kwk::__::for_until( [&](auto... is)
                        {
                          if(f(c(is...)))
                          {
                            pos = kumi::tuple{is...};
                            return true;
                          }
                          return false;
                        }
                      , c.shape()
                      );

    return pos;
  }

  template<typename Container, typename Check>
  auto find_if(Container const& c, Check f)
  {
    return kwk::find_if(cpu, c, f);
  }


  template <typename Context, concepts::container Out>
  constexpr auto find(Context& ctx, Out const& out, auto value)
  {
    return find_if(ctx, out, [&](auto e){return e == value;});
  }

  template <concepts::container Out>
  constexpr auto find(Out const& out, auto value)
  {
    return find(cpu, out, value);
  }


  template <typename Context, typename Func, concepts::container Out>
  constexpr auto find_if_not(Context& ctx, Out const& out, Func f)
  {
    return find_if(ctx, out, [f](auto x){return !f(x);});
  }
  template <typename Func, concepts::container Out>
  constexpr auto find_if_not(Out const& out, Func f)
  {
    return kwk::find_if_not(cpu, out, f);
  }


  template <typename Context, concepts::container Out, concepts::container Values>
  constexpr auto find_first_of(Context& ctx, Out const& out, Values const& in)
  {
    return find_if(ctx, out, [&](auto e)
    {
      return any_of(ctx, in, [&](auto x){return (x==e);});
    });
  }
  template <concepts::container Out, concepts::container Values>
  constexpr auto find_first_of(Out const& out, Values const& in)
  {
    return kwk::find_first_of(cpu, out, in);
  }


  template <typename Context, typename Func, concepts::container Out>
  constexpr auto find_last_if(Context& ctx, Out const& out, Func f)
  {
    auto res = find_if(ctx, kwk::reverse(out), f);

    // TODO: replace kumi::for_each with kwk::for_each?
    kumi::for_each([](auto s, auto& r)
    {
      r = s-1-r;
    }, out.shape(), res);

    return res;
  }
  template <typename Func, concepts::container Out>
  constexpr auto find_last_if(Out const& out, Func f)
  {
    return find_last_if(cpu, out, f);
  }


  template <typename Context, concepts::container Out>
  constexpr auto find_last(Context& ctx, Out const& out, auto value)
  {
    return find_last_if(ctx, out, [&](auto e){return e == value;});
  }
  template <concepts::container Out>
  constexpr auto find_last(Out const& out, auto value)
  {
    return kwk::find_last(cpu, out, value);
  }


  template <typename Context, typename Func, concepts::container Out>
  constexpr auto find_last_if_not(Context& ctx, Out const& out, Func f)
  {
    return find_last_if(ctx, out, [f](auto x){return !f(x);});
  }
  template <typename Func, concepts::container Out>
  constexpr auto find_last_if_not(Out const& out, Func f)
  {
    return kwk::find_last_if_not(cpu, out, f);
  }

}
