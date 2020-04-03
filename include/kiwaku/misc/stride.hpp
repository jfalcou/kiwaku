//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_MISC_STRIDE_HPP_INCLUDED
#define KIWAKU_MISC_STRIDE_HPP_INCLUDED

#include <kiwaku/misc/unit_stride.hpp>
#include <iterator>
#include <cassert>
#include <cstddef>
#include <utility>
#include <iosfwd>
#include <array>

namespace kwk
{
  template<std::size_t Dimensions>
  struct stride : private std::array<std::ptrdiff_t,Dimensions>
  {
    using storage_type = std::array<std::ptrdiff_t,Dimensions>;

    //==============================================================================================
    // NTTP Indirect interface
    //==============================================================================================
    using stride_type = stride<Dimensions>;
    using option_tag  = stride_option;
    static constexpr bool is_dynamic_option   = false;
    static constexpr bool is_unit_stride      = false;
    static constexpr bool is_explicit         = false;

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
    constexpr stride(shape<Dimensions> const& shp) noexcept
    {
      (*this)[0] = 1;
      if constexpr(static_size > 1)
      {
        for(std::size_t i = 1; i<static_size;++i)
          (*this)[i] = (*this)[i-1] * shp[i-1];
      }
    }

    template<typename Stride>
    constexpr stride(detail::explicit_<Stride> const& str) noexcept
            : storage_type( static_cast<storage_type const&>(str) )
    {}

    //==============================================================================================
    // Construct from some amount of integral values
    //==============================================================================================
    template<typename... T>
    constexpr stride(T... s) noexcept
    requires ((std::is_convertible_v<T,std::ptrdiff_t> && ...) && sizeof...(T)<=static_size)
            : storage_type{ static_cast<std::ptrdiff_t>(s)...}
    {}

    //==============================================================================================
    // Sequence interface
    //==============================================================================================
    static constexpr std::size_t    size()  noexcept { return static_size;  }
    static constexpr std::ptrdiff_t count() noexcept { return static_count; }

    template<std::size_t I>
    friend constexpr decltype(auto) get(stride<Dimensions> const& s) noexcept
    {
      return static_cast<storage_type const&>(s)[I];
    }

    template<std::size_t I>
    friend constexpr decltype(auto) get(stride<Dimensions>& s) noexcept
    {
      return static_cast<storage_type&>(s)[I];
    }

    //==============================================================================================
    // I/O
    //==============================================================================================
    friend std::ostream& operator<<(std::ostream& os, stride const& s)
    {
      os << "[[";
      for(auto e : s) os << " " << e;
      os << " ]]";
      return os;
    }

    //==============================================================================================
    // stride interface
    //==============================================================================================
    template<typename... Int> constexpr auto index(Int... is) const noexcept
    {
      return linearize(std::make_index_sequence<sizeof...(Int)>(),is...);
    }

    private:
    template<std::size_t... Idx, typename... Int>
    constexpr auto linearize( std::index_sequence<Idx...> const&, Int... idx ) const noexcept
    {
      return ((idx * get<Idx>(*this)) + ...);
    }
  };
}

namespace std
{
  template<std::size_t I, std::size_t Dimensions>
  struct tuple_element<I, kwk::stride<Dimensions>>
  {
    using type = typename kwk::stride<Dimensions>::value_type;
  };

  template<std::size_t Dimensions>
  struct  tuple_size<kwk::stride<Dimensions>>
        : std::integral_constant<std::size_t,Dimensions>
  {
  };
}

#endif
