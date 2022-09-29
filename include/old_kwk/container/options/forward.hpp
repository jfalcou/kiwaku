//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/raberu.hpp>
#include <kwk/options.hpp>

namespace kwk::options
{
  template<typename Mode, rbr::concepts::settings Settings> struct data;
  template<typename Mode, rbr::concepts::settings Settings> struct element;

  template<typename Tag, rbr::concepts::settings Settings>
  constexpr auto shape(Tag const&, Settings const&) noexcept;

  template<typename Tag, rbr::concepts::settings Settings>
  constexpr auto source(Tag const&, Settings const&) noexcept;

  template<typename Tag, rbr::concepts::settings Settings>
  constexpr auto stride(Tag const&, Settings const&) noexcept;

  template<typename Tag, rbr::concepts::settings Settings>
  constexpr auto offset(Tag const&, Settings const&) noexcept;
}
