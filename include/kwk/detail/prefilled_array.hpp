//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/assert.hpp>
#include <kwk/detail/hybrid_sequence.hpp>
#include <kwk/detail/kumi.hpp>
#include <type_traits>
#include <array>
#include <cstddef>
#include <utility>

namespace kwk::detail
{
  // Compute the prefilled_array base class for prefilled storage
  template<auto Desc> struct array_storage
  {
    using size_map                              = decltype(Desc.size_map());
    static constexpr std::ptrdiff_t static_size = Desc.size();
    using size_type                             = typename decltype(Desc)::size_type;

    struct empty_storage {};
    static constexpr std::ptrdiff_t storage_size  = static_size - size_map::size;
    using type  = std::conditional_t< (storage_size!=0)
                                    , std::array<size_type,storage_size>
                                    , empty_storage
                                    >;
  };

  // prefilled_array is an array containing data or compile-time data
  template<auto Desc>
  struct prefilled_array : detail::array_storage<Desc>::type
  {
    using parent          = detail::array_storage<Desc>;
    using size_map        = typename parent::size_map;
    using size_type       = typename parent::size_type;
    using storage_t       = typename parent::type;
    using is_product_type = void;

    static constexpr std::ptrdiff_t static_size       = Desc.size();
    static constexpr std::ptrdiff_t storage_size      = parent::storage_size;
    static constexpr bool           is_dynamic        = storage_size >= 1;
    static constexpr bool           is_fully_dynamic  = storage_size == static_size;

    constexpr prefilled_array() noexcept {}

    static constexpr auto size() noexcept { return static_size; }

    template<auto Desc2>
    requires( Desc2.size() < static_size || Desc.is_compatible(Desc2) )
    constexpr prefilled_array& operator=( prefilled_array<Desc2> const& p ) noexcept
    {
      prefilled_array that(p);
      swap(that);
      return *this;
    }

    template<std::size_t I>
    requires(I>=0 && I<static_size) constexpr auto get() const noexcept
    {
      if constexpr(size_map::contains(I)) return Desc.at(I);
      else return storage()[size_map::template locate<static_size>(I)];
    }

    template<std::size_t I>
    requires(I>=0 && I<static_size) constexpr decltype(auto) get() noexcept
    {
      if constexpr(size_map::contains(I)) return Desc.at(I);
      else return storage()[size_map::template locate<static_size>(I)];
    }

    constexpr auto operator[](std::size_t i) const noexcept
    {
      if constexpr(static_size == 0) return 1; else return as_array()[i];
    }

    constexpr auto& operator[](std::size_t i) noexcept requires( is_dynamic && static_size>0)
    {
      return storage()[size_map::template locate<static_size>(i)];
    }

    // Swap prefilled_array's contents
    void swap( prefilled_array& other ) noexcept { storage().swap( other.storage() ); }
    friend void swap( prefilled_array& x,prefilled_array& y ) noexcept { x.swap(y); }

    // Conversion to std::array
    constexpr decltype(auto) as_array() const noexcept
    {
      if constexpr(is_fully_dynamic) return storage();
      else
      {
        return kumi::apply( [](auto... m) { return std::array<size_type,static_size>{m...}; }
                          , *this
                          );
      }
    }

    constexpr storage_t&        storage() noexcept       { return static_cast<storage_t&>(*this);        }
    constexpr storage_t const&  storage() const noexcept { return static_cast<storage_t const&>(*this);  }
  };
}

// prefilled_array supports structured bindings.
namespace kwk::detail
{
  template<std::size_t I, auto Desc>
  constexpr auto get(prefilled_array<Desc> const& s) noexcept { return s.template get<I>(); }

  template<std::size_t I, auto Desc>
  constexpr decltype(auto) get(prefilled_array<Desc>& s) noexcept { return s.template get<I>(); }
}

template<std::size_t I, auto Desc>
struct std::tuple_element<I, kwk::detail::prefilled_array<Desc>>
{
  using type = typename kwk::detail::prefilled_array<Desc>::size_type;
};

template<auto Desc>
struct  std::tuple_size<kwk::detail::prefilled_array<Desc>>
      : std::integral_constant<std::size_t,kwk::detail::prefilled_array<Desc>::static_size>
{
};
