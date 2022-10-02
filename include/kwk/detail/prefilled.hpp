//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/assert.hpp>
#include <kwk/detail/combo.hpp>
#include <kwk/detail/pp_helpers.hpp>
#include <array>

namespace kwk::detail
{
  // Compute the prefilled_array base class for prefilled storage
  template<auto Desc> struct array_storage
  {
    using descriptor_t    = decltype(Desc);
    using value_type      = typename descriptor_t::base_type;
    static constexpr  std::size_t storage_size = kumi::count_if(Desc,kumi::predicate<is_joker>());

    struct empty_storage {};
    using storage_type =  std::conditional_t< (storage_size!=0)
                                            , std::array<value_type,storage_size>
                                            , empty_storage
                                            >;
  };

  template<auto Desc> struct prefilled : array_storage<Desc>::storage_type
  {
    using descriptor_t    = decltype(Desc);
    using value_type      = typename descriptor_t::base_type;
    using storage_t       = typename array_storage<Desc>::storage_type;
    using is_product_type = void;

    static constexpr  std::size_t static_size     = std::tuple_size<descriptor_t>::value;
    static constexpr  std::size_t dynamic_size    = kumi::count_if(Desc,kumi::predicate<is_joker>());
    static constexpr  bool        is_fully_static = (dynamic_size == 0);
    static constexpr  bool        is_dynamic      = !is_fully_static;

    //
    static constexpr
    auto index  = kumi::map ( [k=0]<typename V>(V) mutable
                              {
                                if constexpr(kwk::is_joker_v<V>) return k++;
                                else return -1;
                              }
                            , Desc
                            );

    static constexpr
    auto location = kumi::apply ( [](auto... m)
                                  {
                                    return std::array < std::ptrdiff_t
                                                      , static_size
                                                      >{static_cast<std::ptrdiff_t>(m)...};
                                  }
                                , index
                                );

    // Do we have runtime storage for a given index ?
    static bool constexpr contains(std::size_t i) { return location[i] != -1; }

    // Defautl constructor
    constexpr prefilled() : storage_t{} {}

    // Construct using a higher-level filling strategy
    template<typename Filler>
    constexpr prefilled(Filler f)
    {
      kumi::for_each_index( [&]<typename V>(auto i, V const&, auto m)
                            {
                              if constexpr(kwk::is_joker_v<V>) storage()[m] = f(i,m);
                            }
                          , Desc, index
                          );
    }

    // Static access
    template<int N>
    friend KWK_FORCEINLINE constexpr auto get(prefilled const& s) noexcept
    requires(N>=0 && N<static_size)
    {
      if constexpr(is_joker_v<kumi::element_t<N,descriptor_t>>) return s.storage()[location[N]];
      else return get<N>(Desc);
    }

    template<int N>
    friend KWK_FORCEINLINE constexpr auto& get(prefilled& s) noexcept
    requires(N>=0 && N<static_size)
    {
      if constexpr(is_joker_v<kumi::element_t<N,descriptor_t>>) return s.storage()[location[N]];
      else return get<N>(Desc);
    }

    // Dynamic access
    KWK_FORCEINLINE constexpr auto operator[](std::size_t i) const noexcept
    {
      KIWAKU_ASSERT(i<static_size , "[kwk] - Out of bounds access");
      if constexpr(static_size == 0) return 1; else return as_array()[i];
    }

    KWK_FORCEINLINE constexpr auto& operator[](std::size_t i) noexcept
    requires( is_dynamic && static_size>0)
    {
      KIWAKU_ASSERT ( i<static_size , "[kwk] - Out of bounds access"                  );
      KIWAKU_ASSERT ( contains(i)   , "[kwk] - Access overwrites a compile-time value");
      return storage()[location[i]];
    }

    // Total size of the array
    static constexpr auto size() noexcept { return static_size; }

    // Conversion to std::array
    constexpr decltype(auto) as_array() const noexcept
    {
      return kumi::apply( [](auto... m)
                          {
                            return std::array<value_type,static_size>{static_cast<value_type>(m)...};
                          }
                        , *this
                        );
    }

    // Swap prefilled_array's contents
    void swap( prefilled& other ) noexcept { storage().swap( other.storage() ); }
    friend void swap( prefilled& x, prefilled& y ) noexcept { x.swap(y); }

    // Internal storage access for algorithms
    constexpr storage_t&        storage() noexcept
    {
      return static_cast<storage_t&>(*this);
    }

    constexpr storage_t const&  storage() const noexcept
    {
      return static_cast<storage_t const&>(*this);
    }
  };

  // Project  elements in a N dimension prefilled instance
  template<std::size_t N, auto Desc>
  constexpr auto compress(prefilled<Desc> const& s)  noexcept
  {
    using that_t = prefilled<compress<N>(Desc)>;
    that_t that;

    kumi::for_each_index( [&]<typename I>(I, auto m)
                          {
                            if constexpr(I::value < N && that_t::contains(I::value))
                              that[I::value] = m;
                          }
                        , s
                        );

    kumi::for_each_index( [&]<typename I>(I, auto m)
                          {
                            if constexpr(I::value >= N && that_t::contains(N-1))
                              that[N-1] *= m;
                          }
                        , s
                        );
    return that;
  }
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

