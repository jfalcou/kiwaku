//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/detail/abi.hpp>
#include <kwk/detail/kumi.hpp>

#if !defined(KWK_DEFAULT_SHAPE_CONSTRAINTS)
#define KWK_DEFAULT_SHAPE_CONSTRAINTS ::kwk::basic_shape_checks
#endif

namespace kwk
{
  template<auto... D>
  struct shape2;

  template<std::int32_t N, auto... D>
  constexpr auto compress(shape2<D...> const&) noexcept;

  struct basic_shape_checks
  {
    template<auto Dst, auto Src>
    static constexpr bool is_contructible_from()
    {
      if constexpr(Dst.static_size == Src.static_size)
      {
        return Dst.is_equivalent(Src) && Dst.is_compatible(Src);
      }
      else if constexpr(Dst.static_size > Src.static_size)
      {
        return is_contructible_from<Dst.template split<Src.static_size>(),Src>();
      }
      else
      {
        return Dst.is_fully_dynamic;
      }
    }

    template<typename Dst, typename Src>
    static constexpr void construct(Dst& dst, Src const& src)
    {
      if constexpr(Dst::static_size == Src::static_size)
      {
        dst.__base() = src.__base();
      }
      else if constexpr(Dst::static_size > Src::static_size)
      {
        dst = kumi::apply( [](auto... d)
                        {
                           return [&]<int... N>(std::integer_sequence<int, N...>)
                           {
                              return Dst{((void)N,1)...,d...};
                           }(std::make_integer_sequence<int, Dst::static_size - Src::static_size>{});
                        }
                        , src
                        );
      }
      else
      {
        dst = compress<Dst::static_size>(src);
      }
    }

    template<typename Self, typename Other>
    KWK_PURE static constexpr bool fit_constraints(Self const& s, Other const& o) noexcept
    {
      if      constexpr(Other::is_fully_dynamic                         ) return true;
      else if constexpr(Other::static_order    != Self::static_order    ) return false;
      else if constexpr(Other::is_fully_static && Self::is_fully_static ) return o.__base().is_compatible(s.__base());
      else
      {
        return [&]<std::size_t... N>(std::index_sequence<N...>)
        {
          auto check = []<typename A>(A a,auto b) { return std::integral<A> || a == b; };

          return (true && ... && check(get<N>(o), get<N>(s)) );
        }(std::make_index_sequence<Self::static_order>{});
      }
    }
  };
}
