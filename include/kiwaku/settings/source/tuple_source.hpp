//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/settings/source.hpp>
#include <kiwaku/concepts/range.hpp>
#include <kumi/tuple.hpp>

namespace kwk::__
{
  //====================================================================================================================
  /*
    tuple_option : a source for a *product-kind* (SoA) container.

    A product kind (`kwk::as<kumi::tuple<T0,T1,...>>()`) is stored structure-of-arrays: each field
    is its own contiguous fiber in the owning region. The natural source for such a space is a kumi
    tuple of "struct of pointers" -- one pointer per field, each over `T_I`.

    tuple_option<kumi::tuple<P0,P1,...>> holds those per-field pointers and exposes them through the
    same `source_pointer` / `shape_of` accessors the scalar source options do (the pointer accessor
    returns a kumi tuple of per-field pointers; the shape accessor returns the common shape over
    which every fiber extends).
  */
  //====================================================================================================================
  template<typename POINTERS> struct tuple_option;

  template<kwk::concepts::pointer... P> struct tuple_option<kumi::tuple<P...>>
  {
    using type            = kumi::tuple<P...>;
    using source_type     = kumi::tuple<P...>;
    using element_type    = kumi::tuple<std::remove_pointer_t<P>...>;
    using identifier_type = source_id;
    using label_type      = kumi::str;
    using pointer         = kumi::tuple<P...>;

    static constexpr label_type              label() { return kumi::str {"Source"}; }

    template<typename Self> constexpr auto&& operator()(this Self&& self, identifier_type const&)
    {
      using opt_t = std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>,
                                       tuple_option const,
                                       tuple_option>;
      return std::forward_like<Self>(static_cast<opt_t&>(self));
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                         tuple_option const&) noexcept
    {
      return os << "Source: " << kumi::_::typer<source_type>();
    }

    constexpr auto    data() const { return pointers_; }

    kumi::tuple<P...> pointers_;
  };

  //@brief Tuple (struct-of-pointers) option deduction guide
  template<kwk::concepts::pointer... P>
  tuple_option(kumi::tuple<P...>) -> tuple_option<kumi::tuple<P...>>;

  //@brief Helper to retrieve the per-field source pointers of a tuple_option
  template<typename POINTERS> constexpr auto source_pointer(tuple_option<POINTERS> const& source)
  {
    return source.data();
  }

  //@brief Helper to retrieve the shape of a tuple_option (the common shape of every fiber)
  template<typename POINTERS> constexpr auto shape_of(tuple_option<POINTERS> const&)
  {
    return shape {kwk::_};
  }
}

namespace kwk::config
{
  template<kwk::concepts::pointer... P> struct preprocess_source<kumi::tuple<P...>>
  {
    static constexpr auto from(kumi::tuple<P...> t) noexcept
    {
      return kwk::__::tuple_option {kumi::map([](auto p) { return p; }, t)};
    }
  };

  template<kwk::concepts::pointer... P> struct preprocess_source<kumi::tuple<P...>&>
  {
    static constexpr auto from(kumi::tuple<P...>& t) noexcept
    {
      return kwk::__::tuple_option {kumi::map([](auto p) { return p; }, t)};
    }
  };

  template<kwk::concepts::pointer... P> struct preprocess_source<kumi::tuple<P...> const&>
  {
    static constexpr auto from(kumi::tuple<P...> const& t) noexcept
    {
      return kwk::__::tuple_option {kumi::map([](auto p) { return p; }, t)};
    }
  };
}
