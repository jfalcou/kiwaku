//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/concepts/extent.hpp>
#include <kwk/detail/abi.hpp>
#include <kwk/detail/assert.hpp>
#include <kwk/detail/kumi.hpp>
#include <kwk/detail/sequence/combo.hpp>
#include <kwk/utility/fixed.hpp>
#include <kwk/utility/traits/extent.hpp>
#include <array>

namespace kwk::detail
{
  // Compute the prefilled_array base class for prefilled storage
  template<auto Desc, std::size_t Size = kumi::count_if(Desc,kumi::predicate<is_joker>())>
  struct array_storage
  {
    using descriptor_t                        = decltype(Desc);
    using value_type                          = typename descriptor_t::base_type;
    static constexpr std::size_t storage_size = Size;
    using storage_type                        =  std::array<value_type,storage_size>;
  };

  template<auto Desc> struct array_storage<Desc, 0ULL>
  {
    using descriptor_t    = decltype(Desc);
    using value_type      = typename descriptor_t::base_type;
    static constexpr  std::size_t storage_size = 0ULL;

    struct storage_type {};
  };

  template<auto Desc> struct prefilled : array_storage<Desc>::storage_type
  {
    using descriptor_t    = decltype(Desc);
    using value_type      = typename descriptor_t::base_type;
    using storage_t       = typename array_storage<Desc>::storage_type;
    using is_product_type = void;

    static constexpr  auto        descriptor      = Desc;
    static constexpr  std::size_t static_size     = Desc.static_size;
    static constexpr  std::size_t dynamic_size    = kumi::count_if(Desc,kumi::predicate<is_joker>());
    static constexpr  bool        is_fully_static = (dynamic_size == 0);
    static constexpr  bool        is_dynamic      = !is_fully_static;

    static constexpr
    auto index  = kumi::map ( [k=0ULL]<typename V>(V) mutable
                              {
                                if constexpr(kwk::is_joker_v<V>) return k++;
                                else return static_size+1;
                              }
                            , Desc
                            );

    static constexpr
    auto location = kumi::apply ( [](auto... m)
                                  {
                                    return std::array<std::size_t, static_size>{m...};
                                  }
                                , index
                                );

    // Do we have runtime storage for a given index ?
    static bool constexpr contains(std::size_t i) { return location[i] != static_size+1; }

    // Default constructor
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

    // Fill with a sequence of value/joker
    constexpr void fill(concepts::extent<value_type> auto... vals) noexcept
    requires( sizeof...(vals) <= static_size )
    {
      auto& v = storage();

      // Fill storage data with provided size
      kumi::for_each_index( [&]<typename N, typename S>(N, S vs)
      {
        constexpr auto i = N::value;
        if constexpr( std::is_convertible_v<S,value_type> )
        {
          if constexpr(contains(i)) v[location[i]] = static_cast<value_type>(vs);
          else  KIWAKU_ASSERT ( vs == get<i>(descriptor)
                              , "[KWK] - Runtime/Compile-time mismatch in constructor"
                              );
        }
        else
        {
          if constexpr(contains(i)) v[location[i]] = i != 0;
        }
      }
      , kumi::tie(vals...)
      );
    }

    template<int N>
    KWK_FORCEINLINE constexpr auto extent() const noexcept
    {
      if constexpr(is_dynamic_extent_v<N,Desc>) return storage()[location[N]];
      else return fixed<get<N>(Desc)>;
    }

    // Static access
    template<int N>
    friend KWK_FORCEINLINE constexpr auto get(prefilled const& s) noexcept
    requires(N>=0 && N<static_size)
    {
      if constexpr(is_dynamic_extent_v<N,Desc>) return s.storage()[location[N]];
      else return get<N>(Desc);
    }

    template<int N>
    friend KWK_FORCEINLINE constexpr auto& get(prefilled& s) noexcept
    requires(N>=0 && N<static_size)
    {
      if constexpr(is_dynamic_extent_v<N,Desc>) return s.storage()[location[N]];
      else return get<N>(Desc);
    }

    // Dynamic access
    KWK_FORCEINLINE constexpr auto operator[](std::convertible_to<std::size_t> auto i) const noexcept
    {
      auto const idx = static_cast<std::size_t>(i);
      KIWAKU_ASSERT(idx<static_size , "[KWK] - Out of bounds access");
      if constexpr(static_size == 0) return 1; else return as_array()[idx];
    }

    KWK_FORCEINLINE constexpr auto& operator[](std::convertible_to<std::size_t> auto i) noexcept
    requires( is_dynamic && static_size>0)
    {
      auto const idx = static_cast<std::size_t>(i);
      KIWAKU_ASSERT( idx<static_size , "[KWK] - Out of bounds access"                  );
      KIWAKU_ASSERT( contains(idx)   , "[KWK] - Access overwrites a compile-time value");
      return storage()[location[idx]];
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
    using t_t = prefilled<compress<N>(Desc)>;
    using v_t = typename t_t::value_type;
    t_t t;

    kumi::for_each_index
    ( [&](auto i, auto m) { if constexpr(i < N && t_t::contains(i)) t[i] = static_cast<v_t>(m); }, s);

    kumi::for_each_index
    ( [&](auto i, auto m) { if constexpr(i >= N && t_t::contains(N-1)) t[N-1] *= m; }, s);

    return t;
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
