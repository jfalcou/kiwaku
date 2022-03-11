//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/detail/view_builder.hpp>
#include <kiwaku/detail/raberu.hpp>
#include <kiwaku/options.hpp>
#include <type_traits>

namespace kwk
{
  template<typename Type, auto... Os>
  struct  view : detail::view_builder<Type,Os...>::span_type
  {
    using span_type = typename detail::view_builder<Type,Os...>::span_type;

    template<rbr::concepts::option... Opts>
    constexpr view( Opts const&... params )
            : view{rbr::settings(params...)}
    {}

    template<rbr::concepts::option... Opts>
    constexpr view(rbr::settings<Opts...> const& params)
            : span_type{ params[kwk::source].as_span() }
    {}

    constexpr auto settings() const noexcept
    {
      return rbr::merge ( rbr::settings(kwk::source = span_type::data())
                        , rbr::settings(Os...)
                        );
    }

  };

  /// This deduction guide is provided for kwk::view to allow deduction from a list of options
  template<rbr::concepts::option... O>
  view(O const&...) -> view<typename rbr::fetch_t<kwk::source,O...>::span_type::base_type, O{}...>;

  /// This deduction guide is provided for kwk::view to allow deduction from another view's settings
  template<rbr::concepts::option... O>
  view(rbr::settings<O...> const&)
      -> view<typename rbr::fetch_t<kwk::source,O...>::span_type::base_type, O{}...>;
}

