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
#include <kwk/detail/stdfix.hpp>
#include <kwk/utility/joker.hpp>
#include <type_traits>
#include <ostream>

namespace kwk { struct joker; }

namespace kwk::__
{
  // Axis descriptor with a name
  template<auto ID, typename Content = joker>
  struct axis_ : rbr::as_keyword<axis_<ID,Content>>
  {
    using is_product_type = void;
    using axis_kind       = axis_<ID,joker>;
    using content_type    = Content;
    using base_type       = std::int32_t;
    using id_type         = decltype(ID);

    static constexpr auto identifier  = ID;
    static constexpr bool is_dynamic  = !std::integral<content_type>;
    static constexpr bool is_indexed   = std::integral<id_type>;

    // Size info
    static constexpr std::int32_t static_size = 1;
    static constexpr std::int32_t size() noexcept { return static_size; }

    // Indexing info
    static constexpr std::int32_t index() noexcept
    {
      if constexpr(is_indexed) return ID; else return -1;
    }
    static constexpr std::int32_t static_index = index();

    static constexpr axis_kind const base() { return {}; }

    using rbr::as_keyword<axis_<ID,Content>>::operator=;

    constexpr axis_() {}
    constexpr axis_(auto v) : value(v) {}

    template<typename T>
    std::ostream& display(std::ostream& os, T v) const
    {
      if constexpr(is_indexed)  return os << "along<" << ID << ">: " << v;
      else                      return os << ID.value() << ": " << v;
    }

    friend std::ostream& operator<<(std::ostream& os, axis_ a)
    {
      if constexpr(is_indexed)  return os << "along<" << ID << ">";
      else                      return os << ID.value();
      if constexpr(concepts::static_axis<axis_>) os << "[" <<  +a.value << "]";
      return os;
    }

    // Tuple interface
    template<std::size_t I>
    friend constexpr decltype(auto) get(axis_& s) noexcept requires(I==0) { return s; }

    template<std::size_t I>
    friend constexpr decltype(auto) get(axis_ const& s) noexcept requires(I==0) { return s; }

    // Axis combination
    template<auto M, typename C>
    friend KWK_FORCEINLINE constexpr auto compress(axis_ a, axis_<M,C> b) noexcept
    {
      auto v                          = a.value * b.value;
      using v_t                       = decltype(v);
      constexpr bool is_other_indexed = axis_<M,C>::is_indexed;

      if      constexpr(is_indexed && is_other_indexed)
      {
        return axis_<std::min(M,ID),v_t>{v};
      }
      else if constexpr(!is_indexed && is_other_indexed)
      {
        return axis_<ID,v_t>{v};
      }
      else if constexpr(!is_other_indexed)
      {
        return axis_<M,v_t>{v};
      }
    }


    // Axis as static value
    constexpr auto operator[](std::integral auto v) const noexcept
    {
      return axis_<ID,decltype(v)>{v};
    }

    Content value;
  };

  template<auto ID, typename C1, typename C2>
  KWK_TRIVIAL constexpr bool operator==(axis_<ID,C1> a, axis_<ID,C2> b) { return a.value == b.value; }
}

// Pre-made axis objects
namespace kwk
{
  template<std::int32_t N>
  inline constexpr __::axis_<N> along = {};

  template<rbr::literals::str ID>
  inline constexpr __::axis_<ID> axis = {};

  inline constexpr auto height  = axis<"height">;
  inline constexpr auto width   = axis<"width">;
  inline constexpr auto depth   = axis<"depth">;
  inline constexpr auto channel = axis<"channel">;

  // Tuple interface
  template<std::size_t I, auto ID>
  constexpr decltype(auto) get(__::axis_<ID>& s) noexcept requires(I==0) { return s; }

  template<std::size_t I, auto ID>
  constexpr decltype(auto) get(__::axis_<ID> const& s) noexcept requires(I==0) { return s; }

}

// Tuple interface adaptation
template<auto ID, typename Content>
struct  std::tuple_size<kwk::__::axis_<ID,Content>>
      : std::integral_constant<std::int32_t,1>
{};

template<std::size_t N, auto ID, typename Content>
struct std::tuple_element<N, kwk::__::axis_<ID,Content>>
{
  using type = kwk::__::axis_<ID,Content>;
};
