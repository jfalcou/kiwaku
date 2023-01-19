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
#include <kwk/detail/raberu.hpp>
#include <kwk/detail/sequence/combo.hpp>
#include <kwk/utility/fixed.hpp>
#include <kwk/utility/traits/extent.hpp>
#include <array>

namespace kwk::detail
{
  template<typename M>
  struct check_dynamic : std::bool_constant<concepts::dynamic_axis<M>>
  {};

  //================================================================================================
  // Compute the prefilled_array base class for prefilled storage
  //================================================================================================
  template< auto Desc
          , int Size = kumi::count_if(Desc, kumi::predicate<check_dynamic>())
          >
  struct array_storage
  {
    using descriptor_t                = decltype(Desc);
    using value_type                  = typename descriptor_t::base_type;
    static constexpr int storage_size = Size;
    using storage_type                = std::array<value_type,storage_size>;
  };

  template<auto Desc> struct array_storage<Desc, 0>
  {
    using descriptor_t                  = decltype(Desc);
    using value_type                    = typename descriptor_t::base_type;
    static constexpr  int storage_size  = 0;

    struct storage_type {};
  };

  //================================================================================================
  // Main prefilled array structure
  //================================================================================================
  template<auto Desc> struct prefilled : array_storage<Desc>::storage_type
  {
    using descriptor_t    = decltype(Desc);
    using value_type      = typename descriptor_t::base_type;
    using storage_t       = typename array_storage<Desc>::storage_type;
    using is_product_type = void;

    static constexpr  auto  descriptor      = Desc;
    static constexpr  int   static_size     = Desc.static_size;
    static constexpr  int   dynamic_size    = kumi::count_if(Desc,kumi::predicate<check_dynamic>());
    static constexpr  bool  is_fully_static = (dynamic_size == 0);
    static constexpr  bool  is_dynamic      = !is_fully_static;

    // Static localisation of dynamic index
    static constexpr
    auto index  = kumi::map ( [k=0ULL]<typename V>(V) mutable
                              {
                                if constexpr(concepts::dynamic_axis<V>) return k++;
                                else return static_size+1;
                              }
                            , Desc
                            );

    static constexpr
    auto location = kumi::apply ( [](auto... m)
                                  {
                                    return std::array<std::size_t, static_size>{static_cast<std::size_t>(m)...};
                                  }
                                , index
                                );

    // Do we have runtime storage for a given index ?
    static bool constexpr contains(std::size_t i) { return location[i] != static_size+1; }

    // Default constructor
    constexpr prefilled() : storage_t{} {}

    // Construct from a perfectly setup combo
    constexpr prefilled(rbr::concepts::option auto const&... o) : storage_t{}
    {
      auto const s = rbr::settings(o...);
      kumi::for_each( [&]<typename M>(auto v, M m)
                      {
                        if constexpr(concepts::dynamic_axis<M>)
                        {
                          storage()[m.index()] = v;
                        }
                      }
                    , rbr::values<kumi::tuple>(s), rbr::keywords<kumi::tuple>(s)
                    );
    }

    // Construct using a higher-level filling strategy
    template<typename Filler>
    constexpr prefilled(Filler f)
    {
      kumi::for_each_index( [&]<typename V>(int i, V const&, auto m)
                            {
                              if constexpr(concepts::dynamic_axis<V>)
                                storage()[m] = f(i,static_size);
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
          else  KIWAKU_ASSERT (   vs == get<i>(descriptor).value
                              ,   "[KWK] - Runtime/Compile-time mismatch in constructor at index "
                              <<  i << " - Expected " << get<i>(descriptor).value
                              << " but " << vs << " was provided"
                              );
        }
        else
        {
          if constexpr(contains(i)) v[location[i]] = (i != static_size-1);
        }
      }
      , kumi::tie(vals...)
      );
    }

    // Dynamic access
    KWK_FORCEINLINE constexpr auto operator[](std::convertible_to<std::size_t> auto i) const noexcept
    {
      auto const idx = static_cast<std::size_t>(i);
      KIWAKU_ASSERT ( idx<static_size ,   "[KWK] - Out of bounds access at index: "
                                      <<  idx << " for a max. of "
                                      << static_size
                    );
      if constexpr(static_size == 0) return 1; else return as_array()[idx];
    }

    KWK_FORCEINLINE constexpr auto& operator[](std::convertible_to<std::size_t> auto i) noexcept
    requires( is_dynamic && static_size>0)
    {
      auto const idx = static_cast<std::size_t>(i);
      KIWAKU_ASSERT ( idx<static_size ,   "[KWK] - Out of bounds access at index: "
                                      <<  idx << " for a max. of "
                                      << static_size
                    );
      KIWAKU_ASSERT ( contains(idx) ,   "[KWK] - Access at index " << idx
                                    <<  " overwrites a compile-time value of "
                                    << kumi::to_tuple(*this)
                    );
      return storage()[location[idx]];
    }

    // Static tuple access
    template<int N>
    friend KWK_FORCEINLINE constexpr auto get(prefilled const& s) noexcept
    requires(N>=0 && N<static_size)
    {
      if constexpr(is_dynamic_extent_v<N,Desc>) return s.storage()[location[N]];
      else return fixed<get<N>(Desc).value>;
    }

    template<int N>
    friend KWK_FORCEINLINE constexpr auto& get(prefilled& s) noexcept
    requires(N>=0 && N<static_size)
    {
      if constexpr(is_dynamic_extent_v<N,Desc>) return s.storage()[location[N]];
      else return fixed<get<N>(Desc).value>;
    }

    template<concepts::axis A>
    static constexpr auto find_axis(A) noexcept
    {
      return kumi::locate ( Desc.storage, []<typename M>(M) { return is_along_v<M,A>; });
    }

    // Named access via Axis
    template<concepts::axis A>
    constexpr auto operator[](A) const noexcept
    requires(find_axis(A{}) < static_size)
    {
      constexpr auto idx = find_axis(A{});
      if constexpr(is_dynamic_extent_v<idx,Desc>) return storage()[location[idx]];
      else return fixed<get<idx>(Desc).value>;
    }

    //==============================================================================================
    // If an error occurs here, this means you tried to access a shape/stride value by an axis
    // descriptor which is not contained in said shape/stride.
    //  E.g:
    //    kwk::shape< width * height > x;
    //    x[depth] = 4;
    //==============================================================================================
    template<concepts::axis A>
    requires(find_axis(A{}) >= static_size)
    constexpr auto operator[](A) const noexcept = delete;

    // Internal storage access for algorithms
    constexpr storage_t&        storage() noexcept
    {
      return static_cast<storage_t&>(*this);
    }

    constexpr storage_t const&  storage() const noexcept
    {
      return static_cast<storage_t const&>(*this);
    }

    // Total size of the array
    static constexpr int size() noexcept { return static_size; }

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
  };

  // Project elements in a N dimension prefilled instance
  template<int N, auto Desc>
  constexpr auto compress(prefilled<Desc> const& s)  noexcept
  {
    using t_t = prefilled<compress<N>(Desc)>;
    t_t t;

    // Split over the requested dimension
    auto[head,tail] = split(s, kumi::index<prefilled<Desc>::static_size - N>);

    // Copy the required untouched bit
    kumi::for_each_index([&](auto i, auto m) { if constexpr(t_t::contains(i)) t[i] = m; }, tail);

    // Accumulate the compressed dimensions if necessary
    if constexpr(t_t::contains(0))
      t[0] = kumi::fold_left( [](auto acc, auto m) { return acc * m; }, head, t[0]);

    return t;
  }
}

//==================================================================================================
// Tuple interface adaptation
//==================================================================================================
template<auto Desc>
struct  std::tuple_size<kwk::detail::prefilled<Desc>>
      : std::integral_constant<int,kwk::detail::prefilled<Desc>::static_size>
{};

template<std::size_t N, auto Desc>
struct  std::tuple_element<N, kwk::detail::prefilled<Desc>>
{
  using type = std::remove_cvref_t<decltype(get<N>(std::declval<kwk::detail::prefilled<Desc>>()))>;
};
