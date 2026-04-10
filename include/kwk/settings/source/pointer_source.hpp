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
  template<kwk::concepts::pointer P>
  struct pointer_option : source_option<std::remove_pointer_t<std::remove_cvref_t<P>>>
  {
    using source_type = std::remove_cvref_t<P>;
    using base = source_option<std::remove_pointer_t<source_type>>;

    constexpr auto operator()(base::identifier_type) const { return *this; }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                         pointer_option const&) noexcept
    {
      return os << "Source: " << kumi::_::typer<source_type>();
    }
  };

  template<typename P> constexpr auto storage(pointer_option<P> const& source)
  {
    return source.data();
  }

  template<typename P> constexpr auto shape_of(pointer_option<P> const&)
  {
    return shape{kwk::_};
  }
}

namespace kwk::config
{
  template<kwk::concepts::pointer P> struct preprocess_source<P>
  {
    static constexpr auto from(P p) noexcept { return kwk::__::pointer_option<P>{p}; }
  };
}
