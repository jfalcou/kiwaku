//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_MISC_UNIT_STRIDE_HPP_INCLUDED
#define KIWAKU_MISC_UNIT_STRIDE_HPP_INCLUDED

#include <iterator>
#include <cassert>
#include <cstddef>
#include <iosfwd>
#include <tuple>
#include <array>

namespace kwk
{
  struct stride_option {};

  template<std::size_t Dimensions>
  struct unit_stride : private std::array<std::ptrdiff_t,Dimensions-1>
  {
    using storage_type = std::array<std::ptrdiff_t,Dimensions-1>;

    //==============================================================================================
    // NTTP Indirect interface
    //==============================================================================================
    using stride_type = unit_stride<Dimensions>;
    using option_tag  = stride_option;
    static constexpr bool is_dynamic_option = false;

    // TODO: Make it actually unit by not storing the 1
    static constexpr bool is_unit_stride    = true;

    //==============================================================================================
    // Dependent types
    //==============================================================================================
    using value_type              = typename storage_type::value_type;
    using reference               = typename storage_type::reference;
    using const_reference         = typename storage_type::const_reference;

    static constexpr std::size_t    static_size   = Dimensions;
    static constexpr std::ptrdiff_t static_count  = Dimensions;

    //==============================================================================================
    // Constructors
    //==============================================================================================
    template<typename Shape>
    constexpr unit_stride(Shape const& shp) noexcept
    requires (!std::is_convertible_v<Shape,std::ptrdiff_t>)
    {
      if constexpr(static_size > 1)
      {
        storage_type::operator[](0) = shp[1];
      }

      if constexpr(static_size > 2)
      {
        for(std::size_t i = 1; i<static_size;++i)
          storage_type::operator[](i) = storage_type::operator[](i-1) * shp[i+1];
      }
    }

    //==============================================================================================
    // Construct from some amount of integral values
    //==============================================================================================
    template<typename... T>
    constexpr unit_stride(T... s) noexcept
    requires ((std::is_convertible_v<T,std::ptrdiff_t> && ...) && sizeof...(T)<static_size)
            : storage_type{ static_cast<std::ptrdiff_t>(s)...}
    {}

    //==============================================================================================
    // Sequence interface
    //==============================================================================================
    static constexpr std::size_t    size()  noexcept { return static_size;  }
    static constexpr std::ptrdiff_t count() noexcept { return static_count; }

    template<std::size_t I>
    friend constexpr std::ptrdiff_t get(unit_stride<Dimensions> const& s) noexcept
    {
      if    constexpr(I==0) return 1;
      else  return static_cast<storage_type const&>(s)[I-1];
    }

    template<std::size_t I>
    friend constexpr decltype(auto) get(unit_stride<Dimensions>& s) noexcept requires(I!=0)
    {
      return static_cast<storage_type&>(s)[I-1];
    }

    //==============================================================================================
    // I/O
    //==============================================================================================
    friend std::ostream& operator<<(std::ostream& os, unit_stride const& s)
    {
      os << "[[ 1";
      for(auto e : s) os << " " << e;
      os << " ]]";
      return os;
    }

    //==============================================================================================
    // unit_stride interface
    //==============================================================================================
    template<typename I0, typename... Int>
    constexpr auto index(I0 i0, Int... is) const noexcept
    {
      return i0 + linearize(std::make_index_sequence<sizeof...(Int)>(),is...);
    }

    private:
    template<std::size_t... Idx, typename... Int>
    constexpr auto linearize( std::index_sequence<Idx...> const&, Int... idx ) const noexcept
    {
      return ((idx * get<Idx+1>(*this)) + ...);
    }
  };
}

namespace std
{
  template<std::size_t I, std::size_t Dimensions>
  struct tuple_element<I, kwk::unit_stride<Dimensions>>
  {
    using type = typename kwk::unit_stride<Dimensions>::value_type;
  };

  template<std::size_t Dimensions>
  struct  tuple_size<kwk::unit_stride<Dimensions>>
        : std::integral_constant<std::size_t,Dimensions>
  {
  };
}

#endif
