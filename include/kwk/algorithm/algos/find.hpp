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
  // Changement de politique : tous les algos sont désormais dans le contexte
  // pour permettre de les remplacer au besoin (même si ça parait simple,
  // mettons que le find ne repose pas sur find_if sur GPU
  // mais sur quelque chose de plus opti)
  template<typename Context, typename Container, typename Check>
  auto find_if(Context const& ctx, Container const& c, Check f)
  {
    return ctx.find_if(c, f);
  }
  template<typename Container, typename Check>
  auto find_if(Container const& c, Check f)
  {
    return find_if(cpu, c, f);
  }

  template <typename Context, concepts::container Out>
  constexpr auto find(Context const& ctx, Out const& out, auto value)
  {
    return ctx.find(out, value);
  }
  template <concepts::container Out>
  constexpr auto find(Out const& out, auto value)
  {
    return find(cpu, out, value);
  }



  
  template <typename Context, typename Func, concepts::container Out>
  constexpr auto find_if_not(Context const& ctx, Out const& out, Func f)
  {
    return ctx.find_if_not(out, f);
  }
  template <typename Func, concepts::container Out>
  constexpr auto find_if_not(Out const& out, Func f)
  {
    return kwk::find_if_not(cpu, out, f);
  }

  template <typename Context, concepts::container Out, concepts::container Values>
  constexpr auto find_first_of(Context const& ctx, Out const& out, Values const& in)
  {
    return ctx.find_first_of(out, in);
  }
  template <concepts::container Out, concepts::container Values>
  constexpr auto find_first_of(Out const& out, Values const& in)
  {
    return kwk::find_first_of(cpu, out, in);
  }


  // TODO : je n'ai pas pu mettre ça dans base, souci d'import de kwk::reverse

  // template <typename Context, typename Func, concepts::container Out>
  // constexpr auto find_last_if(Context const& ctx, Out const& out, Func f)
  // {
  //   return ctx.find_last_if(out, f);
  // }
  // template <typename Func, concepts::container Out>
  // constexpr auto find_last_if(Out const& out, Func f)
  // {
  //   return kwk::find_last_if(cpu, out, f);
  // }

  // template <typename Context, concepts::container Out>
  // constexpr auto find_last(Context const& ctx, Out const& out, auto value)
  // {
  //   return ctx.find_last(out, value);
  // }
  // template <concepts::container Out>
  // constexpr auto find_last(Out const& out, auto value)
  // {
  //   return kwk::find_last(cpu, out, value);
  // }

  // template <typename Context, typename Func, concepts::container Out>
  // constexpr auto find_last_if_not(Context const& ctx, Out const& out, Func f)
  // {
  //   return ctx.find_last_if_not(out, f);
  // }
  // template <typename Func, concepts::container Out>
  // constexpr auto find_last_if_not(Out const& out, Func f)
  // {
  //   return kwk::find_last_if_not(cpu, out, f);
  // }


  /// ORIGINAL:

  // template<typename Container, typename Check>
  // auto find_if(Container const& c, Check f)
  // {
  //   auto pos = kumi::generate<Container::static_order>(-1);

  //   kwk::__::for_until( [&](auto... is)
  //                       {
  //                         if(f(c(is...)))
  //                         {
  //                           pos = kumi::tuple{is...};
  //                           return true;
  //                         }
  //                         return false;
  //                       }
  //                     , c.shape()
  //                     );

  //   return pos;
  // }

  // template <concepts::container Out>
  // constexpr auto find(Out const& out, auto value)
  // {
  //   return find_if(out, [&](auto e){return e == value;});
  // }

  // template <typename Func, concepts::container Out>
  // constexpr auto find_if_not(Out const& out, Func f)
  // {
  //   return kwk::find_if(out, [f](auto x){return !f(x);});
  // }

  // template <concepts::container Out, concepts::container Values>
  // constexpr auto find_first_of(Out const& out, Values const& in)
  // {
  //   return kwk::find_if(out, [&](auto e)
  //   {
  //     return kwk::any_of(in, [&](auto x){return (x==e);});
  //   });
  // }

  template <typename Func, concepts::container Out>
  constexpr auto find_last_if(Out const& out, Func f)
  {
    auto res = kwk::find_if(kwk::reverse(out), f);

    kumi::for_each([](auto s, auto& r)
    {
      r = s-1-r;
    }, out.shape(), res);

    return res;
  }

  template <concepts::container Out>
  constexpr auto find_last(Out const& out, auto value)
  {
    return find_last_if(out, [&](auto e){return e == value;});
  }

  template <typename Func, concepts::container Out>
  constexpr auto find_last_if_not(Out const& out, Func f)
  {
    return kwk::find_last_if(out, [f](auto x){return !f(x);});
  }

  
}
