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

namespace kwk::__
{
  template<typename M>
  struct check_dynamic : std::bool_constant<M::is_dynamic>
  {};

  //================================================================================================
  // Compute the prefilled_array base class for prefilled storage
  //================================================================================================
  template< auto Desc
          , std::int32_t Size = kumi::count_if(Desc, kumi::predicate<check_dynamic>())
          >
  struct array_storage
  {
    using descriptor_t                          = decltype(Desc);
    using value_type                            = typename descriptor_t::base_type;
    static constexpr std::int32_t storage_size  = Size;
    using storage_type                          = std::array<value_type,storage_size>;
  };

  template<auto Desc> struct array_storage<Desc, 0>
  {
    using descriptor_t                          = decltype(Desc);
    using value_type                            = typename descriptor_t::base_type;
    static constexpr  std::int32_t storage_size = 0;

    struct storage_type { constexpr auto operator==(storage_type const&) const noexcept { return true; } };
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

    static constexpr  auto         descriptor      = Desc;
    static constexpr  std::int32_t static_size     = Desc.static_size;
    static constexpr  auto         dynamic_size    = kumi::count_if(Desc,kumi::predicate<check_dynamic>());
    static constexpr  bool         is_fully_static = (dynamic_size == 0);
    static constexpr  bool         is_dynamic      = !is_fully_static;

    // Static localisation of dynamic index
    static constexpr
    auto index  = kumi::map ( [k=0ULL]<typename V>(V) mutable
                              {
                                if constexpr(V::is_dynamic) return k++;
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
                        if constexpr(M::is_dynamic)
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
      kumi::for_each_index( [&]<typename V>(std::int32_t i, V const&, auto m)
                            {
                              if constexpr(V::is_dynamic)
                                storage()[m] = f(i,static_size);
                            }
                          , Desc, index
                          );
    }

    // Fill with a sequence of value/joker
    constexpr void fill(concepts::extent<value_type> auto... vals) noexcept
    requires(sizeof...(vals) == static_size)
    {
      auto& v = storage();
      if constexpr(dynamic_size == static_size)
      {
        v = {static_cast<value_type>(vals)...};
      }
      else
      {
        [=, this]<std::size_t... I>(std::index_sequence<I...>)
        {
          (
            this->set
            (
              std::integral_constant<unsigned, I>{},
              vals
            ),
            ...
          );
        }(std::make_index_sequence<static_size>{});
      }
    }

    constexpr void fill(concepts::extent<value_type> auto... vals) noexcept
    requires(sizeof...(vals) < static_size)
    {
      [=, this]<int... I>(std::integer_sequence<int, I...>)
      {
        this->fill(((void)I,1)..., vals...);
      }(std::make_integer_sequence<int, static_size - sizeof...(vals)>{});
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

    // Static tuple access impl
    template<std::size_t N>
    KWK_FORCEINLINE constexpr auto __get() const noexcept
    {
      if constexpr(is_dynamic_extent_v<N,Desc>) return storage()[location[N]];
      else return fixed<get<N>(Desc).value>;
    }

    template<std::size_t N>
    KWK_FORCEINLINE constexpr auto& __get() noexcept
    {
      if constexpr(is_dynamic_extent_v<N,Desc>) return storage()[location[N]];
      else return fixed<get<N>(Desc).value>;
    }

    // Static tuple access
    template<std::size_t N>
    friend KWK_FORCEINLINE constexpr auto get(prefilled const& s) noexcept
    requires(N>=0 && N<static_size)
    {
      return s.__get<N>();
    }

    template<std::size_t N>
    friend KWK_FORCEINLINE constexpr auto& get(prefilled& s) noexcept
    requires(N>=0 && N<static_size)
    {
      return s.__get<N>();
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
    constexpr storage_t      & storage()       noexcept { return static_cast<storage_t      &>(*this); }
    constexpr storage_t const& storage() const noexcept { return static_cast<storage_t const&>(*this); }

    // Total size of the array
    static constexpr std::int32_t size() noexcept { return static_size; }

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

    // Set a particular value
    constexpr void set(auto const i, auto const val) noexcept
    {
      if constexpr(contains(i)) storage()[location[i]] = static_cast<value_type>(val);
      else  KIWAKU_ASSERT (   val == get<i>(descriptor).value
                          ,   "[KWK] - Runtime/Compile-time mismatch at index "
                          <<  i << " - Expected " << get<i>(descriptor).value
                          << " but " << val << " was provided"
                          );
    }

    // Swap prefilled_array's contents
    void swap( prefilled& other ) noexcept { storage().swap( other.storage() ); }
    friend void swap( prefilled& x, prefilled& y ) noexcept { x.swap(y); }
  };

  // Project elements in a N dimension prefilled instance
  template<std::int32_t N, auto Desc>
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

namespace kwk
{
  template<std::size_t N,auto Desc>
  KWK_PURE KWK_TRIVIAL constexpr auto get(__::prefilled<Desc> const& s) noexcept
  requires(N>=0 && N<__::prefilled<Desc>::static_size)
  {
    return s.template __get<N>();
  }

  template<std::size_t N,auto Desc>
  KWK_PURE KWK_TRIVIAL constexpr auto& get(__::prefilled<Desc>& s) noexcept
  requires(N>=0 && N<__::prefilled<Desc>::static_size)
  {
    return s.template __get<N>();
  }
}

//==================================================================================================
// Tuple interface adaptation
//==================================================================================================
template<auto Desc>
struct  std::tuple_size<kwk::__::prefilled<Desc>>
      : std::integral_constant<std::int32_t,kwk::__::prefilled<Desc>::static_size>
{};

template<std::size_t N, auto Desc>
struct  std::tuple_element<N, kwk::__::prefilled<Desc>>
{
  using type = std::remove_cvref_t<decltype(get<N>(std::declval<kwk::__::prefilled<Desc>>()))>;
};
