//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//=====================================================================================================================
#pragma once

#include <kwk/concepts/axis.hpp>
#include <kwk/detail/abi.hpp>
#include <raberu/raberu.hpp>
#include <kwk/detail/traits.hpp>
#include <kwk/detail/stdfix.hpp>
#include <kwk/utility/joker.hpp>
#include <type_traits>
#include <ostream>

namespace kwk { struct joker; }

namespace kwk::__
{
  // Axis descriptor with an ID
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
    static constexpr bool is_indexed  = std::integral<id_type>;
    static constexpr bool is_implicit = []()
    {
      if constexpr(std::integral<id_type>) return ID < 0; else return false;
    }();

    static constexpr bool is_explicit = !is_implicit;

    // Equivalence
    template<auto OID, typename Other>
    static constexpr bool is_equivalent(axis_<OID,Other> const&) { return std::same_as<axis_<ID>,axis_<OID>>; }

    // Indexing info
    static constexpr std::int32_t index() noexcept { if constexpr(is_indexed) return ID; else return -1; }
    static constexpr std::int32_t static_index = index();

    static constexpr axis_kind const base() { return {}; }

    using rbr::as_keyword<axis_<ID,Content>>::operator=;

    constexpr axis_(axis_ const&) = default;
    constexpr axis_& operator=(axis_ const&) = default;

    constexpr axis_() {}
    constexpr explicit axis_(auto v) : value(v) {}

    template<typename T>
    std::ostream& display(std::ostream& os, T v) const
    {
      if constexpr(is_indexed)
      {
        if(ID<0)  os << "implicit<" << (-ID-1) << ">: ";
        else      os << "along<"    <<  ID << ">: ";
        return os    << v;
      }
      else return os << ID.value() << ": " << v;
    }

    friend std::ostream& operator<<(std::ostream& os, axis_ a)
    {
      if constexpr(is_indexed)
      {
        if(ID<0)  os << "implicit<" << (-ID-1) << ">";
        else      os << "along<"    <<  ID << ">";
      }
      else                      os << ID.value();

      if constexpr(concepts::static_axis<axis_>)  os << "[" << +a.value << "]";
      else                                        os << "[" <<  a.value << "]";
      return os;
    }

    // Axis combination
    template<auto M, typename C>
    friend KWK_FORCEINLINE constexpr auto compress(axis_ a, axis_<M,C> b) noexcept
    {
      auto v                          = a.value * b.value;
      using v_t                       = decltype(v);
      constexpr bool is_other_indexed = axis_<M,C>::is_indexed;

      if      constexpr( is_indexed && is_other_indexed)  return axis_<std::min(M,ID),v_t>{v};
      else if constexpr(!is_indexed && is_other_indexed)  return axis_<ID,v_t>{v};
      else if constexpr(!is_other_indexed)                return axis_<M,v_t>{v};
    }

    // Axis as static value
    constexpr auto operator[](auto v) const noexcept
    {
      return axis_<ID,decltype(v)>{v};
    }

    Content value = {};
  };

  template<auto ID, typename C1, typename C2>
  KWK_TRIVIAL constexpr bool operator==(axis_<ID,C1> a, axis_<ID,C2> b) { return a.value == b.value; }

  template<auto ID1, auto ID2, typename C1, typename C2>
  KWK_TRIVIAL constexpr bool operator==(axis_<ID1,C1> a, axis_<ID2,C2> b)
  {
    if constexpr(std::same_as<decltype(ID1), decltype(ID2)>)  return (ID1 == ID2) && (a.value == b.value);
    else return false;
  }

  template<std::int32_t N>
  inline constexpr axis_<-N-1> implicit = {};
}

// Pre-made axis objects
namespace kwk
{
  /// Axis specifier for indexed axis
  template<std::int32_t N>
  inline constexpr __::axis_<N> along = {};

  /// Axis specifier for named axis
  template<rbr::literals::str ID>
  inline constexpr __::axis_<ID> axis = {};

  /// Predefined axis for height
  inline constexpr auto height  = axis<"height">;

  /// Predefined axis for width
  inline constexpr auto width   = axis<"width">;

  /// Predefined axis for depth
  inline constexpr auto depth   = axis<"depth">;

  /// Predefined axis for channel
  inline constexpr auto channel = axis<"channel">;
}
