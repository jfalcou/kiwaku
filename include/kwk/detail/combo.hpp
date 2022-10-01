//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/kumi.hpp>
#include <kwk/utility/joker.hpp>
#include <type_traits>

namespace kwk::detail
{
  // combo sequence allow for constructing sequence of static value and placeholders
  template<typename T, typename... Elems> struct combo
  {
    using is_product_type = void;
    using base_type = T;

    constexpr auto operator()() const
    {
      return combo<T,Elems...,joker>{kumi::push_back(data,_)};
    }

    constexpr auto operator[](std::size_t i)  const
    {
      return combo<T,Elems...,std::size_t>{kumi::push_back(data,i)};
    }

    template<std::size_t N>
    friend constexpr decltype(auto) get(combo& s) noexcept { return get<N>(s.data); }

    template<std::size_t N>
    friend constexpr decltype(auto) get(combo const& s) noexcept { return get<N>(s.data); }

    // combo sequence are compatible if they have the same size
    // and don't contains conflicting static values
    template< typename T0, typename... E0>
    constexpr bool is_compatible(combo<T0,E0...> const& other) const noexcept
    {
      if constexpr(sizeof...(Elems) != sizeof...(E0))  return false;
      else
      {
        return kumi::fold_left( [](bool acc, auto const& t)
                                {
                                  auto[e,f] = t;
                                  // _ _ or _ k is OK by design
                                  if constexpr(is_joker_v<decltype(e)>) return acc;
                                  // k _ is OK but checks later
                                  else if constexpr ( !is_joker_v<decltype(e)>
                                                    && is_joker_v<decltype(f)>
                                                    )                   return acc;
                                  // k1 k2 requires e == f
                                  else                                  return acc && (e == f);
                                }
                              , kumi::zip(*this,other)
                              , true
                              );
      }
    }

    kumi::tuple<Elems...> data;
  };

  // Special case for empty combo so to not instantiate kumi::tuple<>
  template<typename T> struct combo<T>
  {
    using base_type = T;
    constexpr auto operator()()               const { return combo<T,joker>{_}; }
    constexpr auto operator[](std::size_t i)  const { return combo<T,std::size_t>{i}; }
  };
}

/*
template<typename ST2,typename... O2>
    constexpr bool is_compatible(hybrid_sequence<ST2,O2...> o) const noexcept
    {
      if constexpr(sizeof...(O2) != sizeof...(Ops))  return false;
      else
      {
        return [&]<std::size_t... I>( std::index_sequence<I...>)
        {
        }( std::make_index_sequence<sizeof...(Ops)>{});
      }
    }
*/
// Tuple interface adaptation
template<typename T, typename... D>
struct  std::tuple_size<kwk::detail::combo<T, D...>>
      : std::integral_constant<std::size_t,sizeof...(D)>
{};

template<std::size_t N, typename T, typename... D>
struct  std::tuple_element<N, kwk::detail::combo<T,D...>> :
        std::tuple_element<N, kumi::tuple<D...>>
{};
