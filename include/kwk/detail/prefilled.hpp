//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/combo.hpp>
#include <array>

namespace kwk::detail
{
  template<auto Desc>
  struct prefilled
  {
    using decriptor_t     = decltype(Desc);
    using value_type      = typename decriptor_t::base_type;
    using is_product_type = void;

    static constexpr  std::size_t static_size     = std::tuple_size<decriptor_t>::value;
    static constexpr  std::size_t dynamic_size    = kumi::count_if(Desc,kumi::predicate<is_joker>());
    static constexpr  bool        is_fully_static = (dynamic_size == 0);
    static constexpr  bool        is_dynamic      = !is_fully_static;

    static constexpr  auto  index = kumi::map ( [k=-1]<typename V>(V const&) mutable
                                                {
                                                  k += kwk::is_joker<V>::value;
                                                  return k;
                                                }
                                              , Desc
                                              );

    static constexpr
    auto location = kumi::apply ( [](auto... m) { return std::array<value_type,static_size>{m...}; }
                                , index
                                );

    constexpr prefilled() : values{} {}

    template<typename Filler>
    constexpr prefilled(Filler f)
    {
      kumi::for_each_index( [&]<typename V, typename I>(I i, V const&, auto m)
                            {
                              if constexpr(kwk::is_joker_v<V>) values[m] = f(i,m);
                            }
                          , Desc, index
                          );
    }

    template<int N>
    friend constexpr decltype(auto) get(prefilled const& s)
    requires(N>=0 && N<static_size)
    {
      if constexpr(is_joker_v<kumi::element_t<N,decriptor_t>>) return s.values[get<N>(index)];
      else return get<N>(Desc);
    }

    template<int N>
    friend constexpr decltype(auto) get(prefilled& s)
    requires(N>=0 && N<static_size)
    {
      if constexpr(is_joker_v<kumi::element_t<N,decriptor_t>>) return s.values[get<N>(index)];
      else return get<N>(Desc);
    }

    constexpr auto operator[](std::size_t i) const noexcept
    {
      if constexpr(static_size == 0) return 1; else return as_array()[i];
    }

    constexpr auto& operator[](std::size_t i) noexcept requires( is_dynamic && static_size>0)
    {
      return values[location[i]];
    }

    static constexpr auto size() noexcept { return static_size; }

    // Conversion to std::array
    constexpr decltype(auto) as_array() const noexcept
    {
      return kumi::apply( [](auto... m) { return std::array<value_type,static_size>{m...}; }
                        , *this
                        );
    }

    // Swap prefilled_array's contents
    void swap( prefilled& other ) noexcept { values.swap( other.storage() ); }
    friend void swap( prefilled& x, prefilled& y ) noexcept { x.swap(y); }

    std::array<value_type,dynamic_size> values;
  };
}

// Tuple interface adaptation
template<auto Desc>
struct  std::tuple_size<kwk::detail::prefilled<Desc>>
      : std::integral_constant<std::size_t,kwk::detail::prefilled<Desc>::static_size>
{};

template<std::size_t N, auto Desc>
struct  std::tuple_element<N, kwk::detail::prefilled<Desc>>
{
  using type = typename kwk::detail::prefilled<Desc>::value_type;
};

/*
    template<auto Desc2>
    requires( Desc2.size() < static_size || Desc.is_compatible(Desc2) )
    constexpr prefilled_array& operator=( prefilled_array<Desc2> const& p ) noexcept
    {
      prefilled_array that(p);
      swap(that);
      return *this;
    }
*/
