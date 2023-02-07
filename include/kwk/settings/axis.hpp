//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/concepts/axis.hpp>
#include <kwk/detail/abi.hpp>
#include <kwk/detail/raberu.hpp>
#include <kwk/detail/traits.hpp>
#include <kwk/utility/joker.hpp>
#include <type_traits>
#include <ostream>

namespace kwk { struct joker; }

namespace kwk::__
{
  // Axis descriptor with a name
  template<rbr::literals::str_ Name, typename Content = joker>
  struct axis_ : rbr::as_keyword<axis_<Name,Content>>
  {
    using is_product_type = void;
    using axis_kind       = axis_<Name,joker>;
    using content_type    = Content;
    using base_type       = std::int32_t;

    // Size info
    static constexpr std::int32_t static_size = 1;
    static constexpr std::int32_t size() noexcept { return static_size; }

    static constexpr std::int32_t static_index  = -1;
    static constexpr std::int32_t index() noexcept { return static_index; }

    static constexpr axis_kind const base() { return {}; }

    using rbr::as_keyword<axis_<Name,Content>>::operator=;

    constexpr axis_() {}
    explicit constexpr axis_(Content c) : value(c) {}

    template<typename T>
    std::ostream& display(std::ostream& os, T v) const { return os << Name.value() << ": " << v; }

    friend std::ostream& operator<<(std::ostream& os, axis_ a)
    {
      os << Name.value();
      if constexpr(concepts::static_axis<axis_>) os << "[" <<  +a.value << "]";
      return os;
    }

    // Tuple interface
    template<std::size_t I>
    friend constexpr decltype(auto) get(axis_& s) noexcept requires(I==0) { return s; }

    template<std::size_t I>
    friend constexpr decltype(auto) get(axis_ const& s) noexcept requires(I==0) { return s; }

    // Axis combination
    template<rbr::literals::str_ M, typename C>
    friend KWK_FORCEINLINE constexpr auto compress(axis_ a, axis_<M,C> b) noexcept
    {
      auto v = a.value * b.value;
      return axis_<M,decltype(v)>{v};
    }

    // Axis as static value
    constexpr auto operator[](std::integral auto N) const noexcept
    {
      return axis_<Name,decltype(N)>{N};
    }

    Content value;
  };

  // Axis descriptor with a numerical index
  template<std::int32_t N, typename Content = joker>
  struct indexed_axis_ : rbr::as_keyword<indexed_axis_<N,Content>>
  {
    using is_product_type = void;
    using axis_kind       = indexed_axis_<N,joker>;
    using content_type    = Content;
    using base_type       = std::int32_t;

    static constexpr std::int32_t static_index  = N;
    static constexpr std::int32_t index() noexcept { return static_index; }

    // Size info
    static constexpr std::int32_t static_size = 1;
    static constexpr std::int32_t size() noexcept { return static_size; }
    static constexpr axis_kind const base() { return {}; }

    constexpr indexed_axis_() {}
    constexpr indexed_axis_(auto v) :value(v) {}

    using rbr::as_keyword<indexed_axis_<N,Content>>::operator=;

    template<typename T>
    std::ostream& display(std::ostream& os, T v) const { return os << "along<" << N << ">: " << v; }

    friend std::ostream& operator<<(std::ostream& os, indexed_axis_ a)
    {
      os << "along<" << N << ">";
      if constexpr(concepts::static_axis<indexed_axis_>) os << "[" <<  +a.value << "]";
      return os;
    }

    // Tuple interface
    template<std::size_t I>
    friend constexpr decltype(auto) get(indexed_axis_& s) noexcept requires(I==0) { return s; }

    template<std::size_t I>
    friend constexpr decltype(auto) get(indexed_axis_ const& s) noexcept requires(I==0) { return s; }

    // Axis combination
    template<std::int32_t M, typename C>
    friend KWK_FORCEINLINE constexpr auto compress(indexed_axis_ a, indexed_axis_<M,C> b) noexcept
    {
      auto v = a.value * b.value;
      return indexed_axis_<std::min(M,N),decltype(v)>{v};
    }

    template<rbr::literals::str_ M, typename C>
    friend KWK_FORCEINLINE constexpr auto compress(indexed_axis_ a, axis_<M,C> b) noexcept
    {
      auto v = a.value * b.value;
      return axis_<M,decltype(v)>{v};
    }

    template<rbr::literals::str_ M, typename C>
    friend KWK_FORCEINLINE constexpr auto compress(axis_<M,C> a, indexed_axis_ b) noexcept
    {
      auto v = a.value * b.value;
      return indexed_axis_<N,decltype(v)>{v};
    }

    // Axis as static value
    constexpr auto operator[](std::integral auto v) const noexcept
    {
      return indexed_axis_<N,decltype(v)>{v};
    }

    Content value;
  };
}

// Pre-made axis objects
namespace kwk
{
  inline constexpr __::axis_<"height">  height  = {};
  inline constexpr __::axis_<"width">   width   = {};
  inline constexpr __::axis_<"depth">   depth   = {};
  inline constexpr __::axis_<"channel"> channel = {};

  template<rbr::literals::str_ Name>
  inline constexpr __::axis_<Name>      axis    = {};

  template<std::int32_t N>
  inline constexpr __::indexed_axis_<N> along   = {};
}

// Tuple interface adaptation
template<std::int32_t N, typename Content>
struct  std::tuple_size<kwk::__::indexed_axis_<N,Content>>
      : std::integral_constant<std::int32_t,1>
{};

template<std::size_t N, std::int32_t I, typename Content>
struct std::tuple_element<N, kwk::__::indexed_axis_<I,Content>>
{
  using type = kwk::__::indexed_axis_<I,Content>;
};

template<rbr::literals::str_ Name, typename Content>
struct  std::tuple_size<kwk::__::axis_<Name,Content>>
      : std::integral_constant<std::int32_t,1>
{};

template<std::size_t N, rbr::literals::str_ Name, typename Content>
struct std::tuple_element<N, kwk::__::axis_<Name,Content>>
{
  using type = kwk::__::axis_<Name,Content>;
};
