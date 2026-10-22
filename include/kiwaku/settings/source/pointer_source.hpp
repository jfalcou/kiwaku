//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

namespace kwk::__
{
  ///@brief pointer_option provides a way to represent a pointer to extract it's properties for view/table
  /// construction
  template<kwk::concepts::pointer P> struct pointer_option : source_option<std::remove_pointer_t<P>>
  {
    using type = P;
    using source_type = std::remove_cvref_t<P>;
    using base = source_option<std::remove_pointer_t<P>>;
    using value_type = std::remove_pointer_t<source_type>;

    template <typename Self>
    constexpr auto&& operator()(this Self&& self, base::identifier_type const&) {
      using pointer_t = std::conditional_t<
        std::is_const_v<std::remove_reference_t<Self>>,
        const pointer_option,
        pointer_option 
      >;
      return std::forward_like<Self>(static_cast<pointer_t&>(self));
    }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                         pointer_option const&) noexcept
    {
      return os << "Source: " << kumi::_::typer<source_type>();
    }
  };

  ///@brief Range option deduction guide
  template<kwk::concepts::pointer P> pointer_option(P p) -> pointer_option<P>;

  ///@brief Helper to retrieve the pointer of a pointer_option
  template<typename P> constexpr auto source_pointer(pointer_option<P> const& source)
  {
    return source.data();
  }

  ///@brief Helper to retrieve a the shape of a pointer_option
  template<typename P> constexpr auto shape_of(pointer_option<P> const&)
  {
    return shape{kwk::_};
  }

}

namespace kwk::config
{
  template<kwk::concepts::pointer P> struct preprocess_source<P>
  {
    static constexpr auto from(P p) noexcept { return kwk::__::pointer_option{p}; }
  };
}
