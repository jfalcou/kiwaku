//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/kumi.hpp>
#include <kwk/detail/abi.hpp>
#include <kwk/utility/joker.hpp>
#include <type_traits>
#include <ostream>

namespace kwk::detail
{
  struct size_;

  // combo sequence allow for constructing sequence of static value and placeholders
  template<typename T, typename... Elems> struct combo
  {
    using is_product_type = void;
    using base_type       = T;
    using contents_type   = kumi::tuple<Elems...>;

    // combo is its self option keyword
    using stored_value_type = combo<T,Elems...>;
    using keyword_type      = detail::size_;

    KWK_FORCEINLINE constexpr auto operator()(keyword_type const&) const noexcept { return *this; }

    static constexpr auto size() noexcept { return sizeof...(Elems); }

    friend std::ostream& operator<<(std::ostream& os, combo c) { return os << c.data; }

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

    // combo sequences are compatible if they have the same size
    // and don't contain conflicting static values
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

    template< typename T0, typename... E0>
    constexpr bool is_similar(combo<T0,E0...> const& other) const noexcept
    {
      if constexpr(sizeof...(Elems) != sizeof...(E0))  return false;
      else
      {
        return kumi::fold_left( [](bool acc, auto const& t)
                                {
                                  auto[e,f] = t;

                                  // _ _ or _ k is OK by design
                                  if constexpr(is_joker_v<decltype(e)>) return acc;
                                  // k _ is KO
                                  else if constexpr ( !is_joker_v<decltype(e)>
                                                    && is_joker_v<decltype(f)>
                                                    )                   return false;
                                  // k1 k2 requires e == f
                                  else                                  return acc && (e == f);
                                }
                              , kumi::zip(*this,other)
                              , true
                              );
      }
    }

    template<typename S2>
    constexpr bool is_similar( S2 const& ) const noexcept
    {
      return is_similar(S2::descriptor);
    }

    kumi::tuple<Elems...> data;
  };

  template<typename T, typename... E>
  constexpr auto to_combo(kumi::tuple<E...> const& es) noexcept
  {
    return combo<T,E...>{es};
  }

  template<typename U, typename T, typename... E>
  constexpr auto combo_cast(combo<T,E...> const& es) noexcept
  {
    return to_combo<U>( kumi::map ( []<typename M>(M m)
                                    {
                                      if constexpr(is_joker_v<M>) return m;
                                      else return static_cast<U>(m);
                                    }
                                  , es.data
                                  )
                      );
  }

  // Compress combo at a given size, projecting remaining elements
  template<std::size_t N, typename T, typename... E>
  constexpr auto compress(combo<T,E...> s)  noexcept
  {
    auto tail = kumi::extract(s.data,kumi::index<N>);

    //  We add a _ to the end of the first part
    //    - if there is at least one _ in the tail
    //    - if there is a _ in the last element of the head
    if constexpr(   kumi::any_of(decltype(tail){},kumi::predicate<is_joker>())
                ||  is_joker_v<kumi::element_t<N-1,typename combo<T,E...>::contents_type>>
                )
    {
      // eg: [1][2][3]()() compress 2 -> [1]()
      auto head = kumi::extract(s.data, kumi::index<0>, kumi::index<N-1>);
      return to_combo<std::ptrdiff_t>(kumi::push_back(head,kwk::_));
    }
    else
    {
      // if there is no joker, we compute the product of all remaining elements and try
      // to insert it in the last part eg: [1][4][5] compress 2 -> [1][20]
      auto value = kumi::fold_left( [](auto acc, auto v) { return acc*v; }
                                  , tail
                                  , get<N-1>(s.data)
                                  );
      return to_combo<std::ptrdiff_t> ( kumi::push_back ( kumi::extract ( s.data
                                                                        , kumi::index<0>
                                                                        , kumi::index<N-1>
                                                                        )
                                                        , value
                                                        )
                                      );
    }
  }

  // Special case for empty combo so to not instantiate kumi::tuple<>
  template<typename T> struct combo<T>
  {
    using base_type = T;
    constexpr auto operator()()               const { return combo<T,joker>{_}; }
    constexpr auto operator[](std::size_t i)  const { return combo<T,std::size_t>{i}; }

    // combo is its self option keyword
    using stored_value_type = combo<T>;
    using keyword_type      = detail::size_;

    KWK_FORCEINLINE constexpr auto operator()(keyword_type const&) const noexcept { return *this; }
  };
}

// Tuple interface adaptation
template<typename T, typename... D>
struct  std::tuple_size<kwk::detail::combo<T, D...>>
      : std::integral_constant<std::size_t,sizeof...(D)>
{};

template<std::size_t N, typename T, typename... D>
struct  std::tuple_element<N, kwk::detail::combo<T,D...>> :
        std::tuple_element<N, kumi::tuple<D...>>
{};
