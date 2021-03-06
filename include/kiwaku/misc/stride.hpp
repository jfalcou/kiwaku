//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/detail/ct_helpers.hpp>
#include <kiwaku/detail/raberu.hpp>
#include <array>
#include <cstddef>
#include <ostream>
#include <type_traits>
#include <utility>

namespace kwk
{
  namespace detail
  {
    //==============================================================================================
    // Build the static storage of a stride depending on # of dimensions and unit dimensions
    //==============================================================================================
    template<std::size_t Dimensions, typename UnitIndices>
    using stride_storage = std::array<std::ptrdiff_t, Dimensions-UnitIndices::size>;

    //==============================================================================================
    // Type representing a unit stride
    //==============================================================================================
    using unit_type = std::integral_constant<std::ptrdiff_t,1>;

    //==============================================================================================
    // Compute an index_list from a pack of stride value types
    //==============================================================================================
    template<typename... Vs> struct index_map : type_map<unit_type, Vs...> {};

    //==============================================================================================
    // Recurring linearize pattern: used y stride and other components
    //==============================================================================================
    template<typename Data, std::size_t... Idx, typename... Is>
    constexpr auto linearize( std::index_sequence<Idx...> const&, Data const& d, Is... is ) noexcept
    {
      using std::get;
      if constexpr(sizeof...(Idx) > 0)  return ((is * get<Idx>(d)) + ...);
      else                              return 0;
    }
  }

  template<auto Shaper> struct shape;

  template<std::size_t Dimensions, typename UnitIndices = detail::index_list<0>>
  struct stride
  {
    //==============================================================================================
    // NTTP Indirect interface
    //==============================================================================================
    using stride_type = stride<Dimensions, detail::index_list<0>>;
    static constexpr bool is_dynamic  = false;
    static constexpr bool is_unit     = UnitIndices::contains(0);
    static constexpr bool is_explicit = false;

    //==============================================================================================
    // Sequence interface
    //==============================================================================================
    using storage_type    = detail::stride_storage<Dimensions,UnitIndices>;
    using value_type      = typename storage_type::value_type;

    static constexpr std::ptrdiff_t size() noexcept { return static_size; }
    static constexpr std::ptrdiff_t static_size  = Dimensions;

    //==============================================================================================
    // Element access
    //==============================================================================================
    template<std::size_t I> constexpr auto get() const noexcept
    {
      constexpr auto i = std::min(I,Dimensions-1);

      if constexpr(UnitIndices::contains(i))
        return std::integral_constant<std::ptrdiff_t,1>{};
      else
        return storage_[UnitIndices::template locate<Dimensions>(i)];
    }

    //==============================================================================================
    // Construct from some amount of integral values
    //==============================================================================================
    template<typename... Values>
    constexpr stride(Values... v) noexcept
          requires( std::is_convertible_v<Values,std::ptrdiff_t> && ...)
        : storage_{}
    {
      constexpr auto sz = sizeof...(Values);

      // Filter out the non-dynamic stride values
      auto ptr = storage_.data();
      detail::for_each_args(  [&ptr](auto sv)
                              {
                                using v_t = std::remove_cvref_t<decltype(sv)>;
                                if constexpr( !std::is_same_v<v_t,detail::unit_type> )
                                  *ptr++ = sv;
                              }
                            , v...
                            );

      // Repeat last stride for when the data are missing
      if constexpr(sz < Dimensions-1)
      {
        detail::constexpr_for<sz,Dimensions-1>
        (
          [&]<std::ptrdiff_t I>( std::integral_constant<std::ptrdiff_t,I> const&)
          {
            storage_[I] = storage_[sz-1];
          }
        );
      }
    }

    //==============================================================================================
    // Construct from a shape with same dimensions
    // Only possible if stride is unit on the inner dimension
    //==============================================================================================
    template<auto Shaper>
    constexpr explicit stride(shape<Shaper> const& shp) noexcept requires(is_unit)
    {
      if constexpr(static_size > 1)
      {
        storage_[0] = shp.template get<0>();
        detail::constexpr_for<1,static_size-1>
        (
          [&]<std::ptrdiff_t I>( std::integral_constant<std::ptrdiff_t,I> const&)
          {
            storage_[I] = storage_[I-1] * shp.template get<I>();
          }
        );
      }
    }

    void swap( stride& other ) noexcept
    {
      storage_.swap( other.storage_ );
    }

    //==============================================================================================
    // indexing interface
    //==============================================================================================
    template<typename... Is>
    constexpr auto  index(Is... is) const noexcept
                    requires(  (  sizeof...(Is) <= Dimensions)
                            && (  std::is_convertible_v<Is,std::ptrdiff_t> && ...)
                            )
    {
      return detail::linearize(std::make_index_sequence<sizeof...(Is)>(),*this,is...);
    }

    //==============================================================================================
    // I/O
    //==============================================================================================
    friend std::ostream& operator<<(std::ostream& os, stride const& s)
    {
      os << "[{ ";

      []<std::size_t... P>( std::index_sequence<P...> const&, auto& stream, auto const& c)
      {
        ((stream << c.template get<P>() << " "),...);
      }( std::make_index_sequence<static_size>{}, os, s);

      os << "}]";
      return os;
    }

    storage_type storage_;
  };

  //================================================================================================
  // Specialization for 1D unit stride
  //================================================================================================
  template<> struct stride<1ULL, kwk::detail::index_list<0ULL>>
  {
    //==============================================================================================
    // NTTP Indirect interface
    //==============================================================================================
    using stride_type = stride<1, detail::index_list<0>>;
    static constexpr bool is_dynamic  = false;
    static constexpr bool is_unit     = true;
    static constexpr bool is_explicit = false;

    //==============================================================================================
    // Sequence interface
    //==============================================================================================
    using value_type = detail::unit_type;

    static constexpr std::ptrdiff_t size() noexcept { return 1; }
    static constexpr std::ptrdiff_t static_size  = 1;

    //==============================================================================================
    // Element access
    //==============================================================================================
    template<std::size_t I> constexpr detail::unit_type get() const noexcept
    {
      return detail::unit_type{};
    }

    //==============================================================================================
    // Construct from some amount of integral values
    //==============================================================================================
    template<typename... Values> constexpr stride(Values...) noexcept {}

    //==============================================================================================
    // indexing interface
    //==============================================================================================
    constexpr auto index(std::ptrdiff_t is) const noexcept { return is; }

    void swap( stride& ) noexcept {}

    //==============================================================================================
    // I/O
    //==============================================================================================
    friend std::ostream& operator<<(std::ostream& os, stride<1,detail::index_list<0>> const&)
    {
      return os << "[{ 1 }]";
    }
  };

  //================================================================================================
  // Unit stride type
  //================================================================================================
  template<std::size_t Dimensions>
  using unit_stride = stride<Dimensions, detail::index_list<0>>;

  //================================================================================================
  // Deduction guides
  //================================================================================================
  template<typename... V>
  stride(V const&...) -> stride<  sizeof...(V)
                                , typename detail::index_map<std::remove_cvref_t<V>...>::type
                                >;

  template<auto Shaper> stride(shape<Shaper> const&...) -> stride<Shaper.size(), detail::index_list<0>>;

  //================================================================================================
  // Structured binding supports
  //================================================================================================
  template<std::size_t I, std::size_t Dimensions,typename IL>
  constexpr auto get(stride<Dimensions,IL> const& s) noexcept
  {
    return s.template get<I>();
  }
}

//==================================================================================================
// Structured binding supports
//==================================================================================================
namespace std
{
  template<std::size_t I, std::size_t Dimensions,typename IL>
  struct tuple_element<I, kwk::stride<Dimensions,IL>>
  {
    using type = decltype(std::declval<kwk::stride<Dimensions,IL>>().template get<I>());
  };

  template<std::size_t Dimensions,typename IL>
  struct  tuple_size<kwk::stride<Dimensions,IL>>
        : std::integral_constant<std::size_t,Dimensions>
  {};
}
