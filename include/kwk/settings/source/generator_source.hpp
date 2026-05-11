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
  template<kwk::concepts::callable C> struct generator_option : source_option < callable < C >>>
  {
    using type = C;
    using source_type = std::remove_cvref_t<C>;
    using base = source_option<callable<C>>;
    using value_type = decltype(kumi::invoke(std::declval<C>()));

    constexpr auto operator()(base::identifier_type) const { return *this; }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                         pointer_option const&) noexcept
    {
      return os << "Source: " << kumi::_::typer<source_type>();
    }
  };

  ///@brief Range option deduction guide
  template<kwk::concepts::callable P> generator_option(P p) -> generator_option<P>;

  ///@brief Helper to retrieve the pointer of a pointer_option
  template<typename P> constexpr auto source_pointer(generator_option<P> const& source)
  {
    return source.data();
  }

  ///@brief Helper to retrieve a the shape of a pointer_option
  template<kwk::concepts::callable C> constexpr auto shape_of(kwk::generator_option<C> const&)
  {
    return shape{kwk::_};
  }

}

namespace kwk::config
{
  template<kwk::concepts::callable C> struct preprocess_source<C>
  {
    static constexpr auto from(C const& c) noexcept { return kwk::__::generator_option{c}; }
  };
}
