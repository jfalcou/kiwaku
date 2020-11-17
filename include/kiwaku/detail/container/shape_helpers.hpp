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
#include <kiwaku/detail/options/stride_option.hpp>
#include <type_traits>
#include <cstddef>
#include <array>

namespace kwk::detail
{
  // ================================== SHAPE ================================= //
  // A tag marking the function call operator
  struct dynamic_size { dynamic_size(auto) {} };

  // A tag marking the subscripting operator
  struct static_size {};

  // The type of a shape remembering the sequence of operations it was made from
  template <typename... Ops> struct shaper
  {
    //==============================================================================================
    // NTTP Option tag
    //==============================================================================================
    static constexpr auto size_map() { return typename type_map<static_size, Ops...>::type{}; }
    static constexpr bool is_fully_static   = size_map().size == sizeof...(Ops);

    //==============================================================================================
    // Main interface
    //==============================================================================================
    using type = shaper<Ops...>;

    constexpr shaper() noexcept = default;

    template <typename... X> constexpr auto append(X... x) const noexcept
    {
      return [&]<std::size_t... I>(std::index_sequence<I...> const&)
      {
        using that_t = std::array<std::ptrdiff_t, sizeof...(Ops) + sizeof...(X)>;
        return that_t{{std::get<I>(data_)..., x...}};
      }(std::index_sequence_for<Ops...>());
    }

    template <typename... Args>
    constexpr shaper(shaper<Args...> other, std::ptrdiff_t i) noexcept : data_(other.append(i))
    {}

    // Add a dynamic dimension
    constexpr auto operator()() const noexcept
    {
      return shaper<Ops..., dynamic_size>(*this, -1);
    }

    // Add a static dimension of known size
    constexpr auto operator[](std::size_t i)  const noexcept
    {
      return shaper<Ops..., static_size >(*this,  i);
    }

    static constexpr std::size_t size() noexcept { return sizeof...(Ops); }

    constexpr std::ptrdiff_t at(std::size_t i) const noexcept { return data_[i]; }

    // Implementation details
    std::array<std::ptrdiff_t, sizeof...(Ops)> data_;
  };

  template<> struct shaper<>
  {
    //==============================================================================================
    // NTTP Option tag
    //==============================================================================================
    static constexpr auto size_map() { return index_list{}; }
    static constexpr bool is_fully_static = true;

    using type = shaper<>;

    template <typename... X> constexpr auto append(X... x) const noexcept
    {
      using that_t = std::array<std::ptrdiff_t, sizeof...(X)>;
      return that_t{{x...}};
    }

    constexpr shaper() noexcept = default;

    constexpr auto operator()() const noexcept
    {
      return shaper<dynamic_size>(*this, -1);
    }

    constexpr auto operator[](std::size_t i)  const noexcept
    {
      return shaper<static_size >(*this,  i);
    }

    static constexpr std::size_t size() noexcept { return 0; }
  };

  //================================================================================================
  // RBR option global tag
  //================================================================================================
  struct shape_tag;
}

//================================================================================================
// Register a RBR keyword
//================================================================================================
namespace kwk::option
{
  inline constexpr auto shape  = ::rbr::keyword<kwk::detail::shape_tag>;
}

namespace rbr
{
  //================================================================================================
  // Register as RBR option
  //================================================================================================
  template <typename... Ops>
  struct tag<kwk::detail::shaper<Ops...>> : tag<kwk::detail::shape_tag> {};
}